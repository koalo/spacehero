/* 
 * This file is part of Spacehero.
 * 
 * Spacehero is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Spacehero is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Spacehero.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _UNIVERSE_H_
#define _UNIVERSE_H_

#include <fstream>
#include <iostream>
#include <iterator>

#include <vector>
#include <math.h>

#include "local.h"

#include <boost/progress.hpp>
#include <iostream>
#include <climits>

#include "SDL/SDL.h" 

using boost::timer;
using boost::progress_timer;
using boost::progress_display;

namespace Error {
  class ParseLevel {
    const char *p;
    public:
    ParseLevel(const char* q): p(q) {}
    const char* msg() {return p;}
  };
}

class SkyObject {
  public:
    double x, y, z; /* in 600000 LJ (0.5 = 300000 LJ = Mitte vom Spielfeld) */
    bool exists;
    bool level;
    double radius;
  public:
    SkyObject():x(0),y(0),z(0),exists(true),level(false),radius(0) {};
    SkyObject(double ix, double iy, double iradius) : x(ix), y(iy), z(0), exists(true), level(false), radius(iradius) {};
    virtual ~SkyObject() {};
    void setlevel() { level = true; };
    bool setexists(bool b=true) { return exists = b; };
    bool getexists() { return exists; };
    void setX(double ix){x = ix;}
    void setY(double iy){y = iy;}
    void setZ(double iz){z = iz;}
    double getX(){return x;}
    double getY(){return y;}
    double getZ(){return z;}
  public:
    friend std::ostream& operator<< (std::ostream &o, const SkyObject &g);
};

class SkyMass : public SkyObject {
  public:
    //double fx, fy, fz; /* Kraft in Newton auf den Koerper */
    double vx, vy, vz; /* in m/s */
    double mass; /* in Sonnenmassen */
  public:
    SkyMass(): vx(0),vy(0),vz(0),mass(0) {};
    SkyMass(double ix, double iy, double imass, double iradius) : SkyObject(ix,iy,iradius), vx(0), vy(0), vz(0), mass(imass) {};
    
    inline void newton(SkyMass &m, double &delta);
    inline void move(double delta);
    void setVX(double ivx){vx = ivx;}
    void setVY(double ivy){vy = ivy;}
    double getVX(){return vx;}
    double getVY(){return vy;}

  public:
    friend std::ostream& operator<< (std::ostream &o, const SkyMass &g);
};

class Goal : public SkyObject {
  public:
    Goal() {};
    Goal(std::ifstream &in);
    void setRadius(double iradius){radius = iradius;}
  public:
    friend std::ostream& operator<< (std::ostream &o, const Goal &g);
};

class Blackhole : public SkyMass {
  public:
    Blackhole(std::ifstream &in);
    Blackhole(double ix, double iy, double imass) : SkyMass(ix, iy, imass, HOLESIZE*sqrt(imass/HOLEMEDIUMMASS)) {};
  public:
    friend std::ostream& operator<< (std::ostream &o, const Blackhole &g);
};

class Star;

class Galaxy : public SkyMass {
    bool master;
  public:
    bool lr; // links true /rechts
  private:
    static const double R_MIN_CENTER = 0.02;
    static const double R_MIN = 0.0005;

    Galaxy();
  public:
    Galaxy(std::ifstream &in);
    Galaxy(double ix, double iy, double imass, bool imaster, bool ilr) : SkyMass(ix, iy, imass, BULGESIZE), master(imaster), lr(ilr) {};

    std::vector<Star> getStars(int seed);
    bool getmaster() { return master; };

  public:
    friend std::ostream& operator<< (std::ostream &o, const Galaxy &g);
};

class Star : public SkyMass {
  public:
    Star(Galaxy &g,double R, double phi, double z, double v, double mass=1e3);
  public:
    friend std::ostream& operator<< (std::ostream &o, const Star &g);
};

class Sky 
{
  protected:
    int seed;
  public:
    std::vector<Star> stars;
    std::vector<Galaxy> galaxies;
    Sky() :seed(0),stars(),galaxies() {};
    virtual ~Sky() {};
    void calcStars();
};

class Level : public Sky
{
    unsigned int t0;
    double maxtime;
    double lastt;
    double m_delta;

    double m_fpst;
    double m_fps;
    std::string name;
  public:
    std::vector<Blackhole> holes;
    Goal goal;
  public:
    Level(std::ifstream &in);
    Level():t0(),maxtime(30.0),lastt(0),m_delta(0),m_fpst(0),m_fps(0),name(""),holes(),goal() {};
    virtual ~Level() {};
    void setName(std::string iname){name = iname;}
    std::string getName(){return name;}

  public:
    void tinit() { t0 = SDL_GetTicks(); }; // start time measure
    double elapsed() { return (SDL_GetTicks()-t0)/1000.0; }; // get elapsed time

    void tick() { lastt=elapsed(); };
    void tack(double weight=0.9) {
      m_delta = weight*m_delta + (1-weight)*(elapsed()-lastt);
    //  lastt=elapsed(); 
    }; // measure round time

    double ldelta(double weight=0.1 ) { return weight*m_delta + (1-weight)*(elapsed()-lastt); };
    double delta() { return m_delta; }; // filtered delta

    double getmaxtime() const { return maxtime; }; // get maxtime
    bool timeout() {return elapsed() > maxtime; }; // TODO: fix to use summed movement time

    double fps() {
      //m_fps = weight*m_fps * (1-weight)*(elapsed() - m_fpst);
      //m_fpst = elapsed();
      return 1/m_delta;
    }

  public:
    friend std::ostream& operator<< (std::ostream &o, const Level &l);
};


class Universe: public Level
{
  bool m_won;
  bool stargrav;
  public:
  Universe(Level &l);
  Universe() :m_won(false),stargrav(false) {};

  //bool play(GLdisplay &d);
  void move(double delta);
  bool won();
  void eventHorizon();
  void setStargrav(bool igra){stargrav = igra;}
};

inline void SkyMass::move(double delta) {
  x += (vx/WIDTHINMETERS)*TIMESCALE*delta;
  y += (vy/WIDTHINMETERS)*TIMESCALE*delta;
  z += (vz/WIDTHINMETERS)*TIMESCALE*delta;
  //if(hypot(vx,vy)>1) std::cerr << ".";
}

inline void SkyMass::newton(SkyMass &m, double &delta) {
  double AX, AY, AZ, a1, a2, r3;
  //std::cerr << *this << m << std::endl;
  if( (!getexists()) || (!m.getexists()) ) return;

  AX = x - m.x;
  AY = y - m.y;
  AZ = z - m.z;

  r3 = sqrt(AX*AX+AY*AY+AZ*AZ);
  r3 = r3*r3*r3;

  AX = AX/r3;
  AY = AY/r3;
  AZ = AZ/r3;

  a1 = SUNGRAVTIMEWIDTH*mass*delta;
  m.vx += a1*AX;
  m.vy += a1*AY;  
  m.vz += a1*AZ;  

  a2 = SUNGRAVTIMEWIDTH*m.mass*delta;
  vx -= a2*AX;
  vy -= a2*AY;
  vz -= a2*AZ;
};

#endif
