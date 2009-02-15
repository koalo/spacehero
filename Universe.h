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



using boost::timer;
using boost::progress_timer;
using boost::progress_display;

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
    
    inline void newton(SkyMass &m, double delta);
    inline void move(double delta);
  public:
    friend std::ostream& operator<< (std::ostream &o, const SkyMass &g);
};

class Goal : public SkyObject {
  public:
    Goal() {};
    Goal(std::ifstream &in);
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

  public:
    Galaxy(std::ifstream &in, bool master=false);

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

class Level {
    timer t0;
    double maxtime;
    double lastt;
    double m_delta;
  public:
    std::vector<Blackhole> holes;
    std::vector<Galaxy> galaxies;
    Goal goal;
  protected:
    int seed;
  public:
    Level(std::ifstream &in);
    virtual ~Level() {};

  public:
    void tick() { t0 = timer(); }; // start time measure
    double tack(double weight=0.99) {
      m_delta = weight*m_delta + (1-weight)*(elapsed()-lastt);
      return lastt=elapsed(); 
    }; // get elapsed time
    double elapsed() { return t0.elapsed(); }; // get elapsed time
    double delta() {
      return m_delta;
    };
    bool timeout() {return t0.elapsed() > maxtime; };

  public:
    friend std::ostream& operator<< (std::ostream &o, const Level &l);
};


class Universe: public Level
{
  bool m_won;
  public:
  std::vector<Star> stars;  
  public:
  Universe(Level &l);
  //bool play(GLdisplay &d);
  void move(double delta);
  bool won();
  void eventHorizon();

};

inline void SkyMass::move(double delta) {
  x += (vx/WIDTHINMETERS)*TIMESCALE*delta;
  y += (vy/WIDTHINMETERS)*TIMESCALE*delta;
  z += (vz/WIDTHINMETERS)*TIMESCALE*delta;
  //if(hypot(vx,vy)>1) std::cerr << ".";
}

inline void SkyMass::newton(SkyMass &m, double delta) {
  double AX, AY, a1, a2, r3;
  //std::cerr << *this << m << std::endl;
  if( (!getexists()) || (!m.getexists()) ) return;

  AX = x - m.x;
  AY = y - m.y;

  r3 = hypot(AX,AY);
  r3 = r3*r3*r3;

  AX = AX/r3;
  AY = AY/r3;

  a1 = SUNGRAVTIMEWIDTH*mass*delta;
  m.vx += a1*AX;
  m.vy += a1*AY;  

  a2 = SUNGRAVTIMEWIDTH*m.mass*delta;
  vx -= a2*AX;
  vy -= a2*AY;
};

#endif
