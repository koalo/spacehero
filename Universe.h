#ifndef _UNIVERSE_H_
#define _UNIVERSE_H_

#include <fstream>
#include <iostream>
#include <iterator>

#include <vector>
#include <math.h>

#include "local.h"

class SkyObject {
  public:
    double x, y, z; /* in 600000 LJ (0.5 = 300000 LJ = Mitte vom Spielfeld) */
    bool exists;
    bool level;
  public:
    SkyObject():x(0),y(0),z(0),exists(true),level(false) {};
    void setlevel() { level = true; };
    friend std::ostream& operator<< (std::ostream &o, const SkyObject &g);
};

class SkyMass : public SkyObject {
  public:
    //double fx, fy, fz; /* Kraft in Newton auf den Koerper */
    double vx, vy, vz; /* in m/s */
  public:
    double mass; /* in Sonnenmassen */
  public:
    SkyMass():
      //fx(0),fy(0),fz(0),
      vx(0),vy(0),vz(0),mass(0) {};
    friend std::ostream& operator<< (std::ostream &o, const SkyMass &g);

    inline void operator^ (SkyMass &m) {
      double AX, AY, a1, a2, r3;
      //std::cerr << *this << m << std::endl;

      AX = x - m.x;
      AY = y - m.y;

      r3 = hypot(AX,AY);
      r3 = r3*r3*r3;

      AX = -AX/r3;
      AY = -AY/r3;

      a1 = SUNGRAVTIMEWIDTH*mass;
      vx += a1*AX;
      vy += a1*AY;  

      a2 = SUNGRAVTIMEWIDTH*m.mass;
      m.vx -= a2*AX;
      m.vy -= a2*AY;
    };
    inline void move() {
      x += (vx/WIDTHINMETERS)*TIMESCALE;
      y += (vy/WIDTHINMETERS)*TIMESCALE;
      z += (vz/WIDTHINMETERS)*TIMESCALE;
      //if(hypot(vx,vy)>1) std::cerr << ".";
    }
};


class Goal : public SkyObject {
  public:
    double radius;
    Goal(): radius(0) {};
    Goal(std::ifstream &in) {
      in >> x >> y >> z;
      in >> radius;
		setlevel();
    };
    friend std::ostream& operator<< (std::ostream &o, const Goal &g);
};

class Blackhole : public SkyMass {
  public:
    Blackhole(std::ifstream &in) {
      double t;
      in >> x >> y >> z; 
      in >> t >> t >> t; 
      in >> mass; 
		setlevel();
    };
    friend std::ostream& operator<< (std::ostream &o, const Blackhole &g);
};

class Star;

class Galaxy : public SkyMass {
  private:
    static const int ORBITS_MAX = 130;
    static const int R_MIN_CENTER = 0.02;
    static const int R_MIN = 0.0005;
    static const int A = 200;
    static const int M = 5;

  public:
    Galaxy(std::ifstream &in) {
      in >> x >> y >> z; 
      in >> vx >> vy >> vz; 
      in >> mass; 
      lr=true;
    };
    bool lr; // links true /rechts

    std::vector<Star> getStars(int seed);

  public:
    friend std::ostream& operator<< (std::ostream &o, const Galaxy &g);
};

class Star : public SkyMass {
  public:
	Star(Galaxy &g,double R, double phi, double z, double v, double mass=1e3);
    friend std::ostream& operator<< (std::ostream &o, const Star &g);
};

class Level {
  public:
    std::vector<Blackhole> holes;
    std::vector<Galaxy> galaxies;
    Goal goal;
  protected:
    int seed;
  public:
    Level(std::ifstream &in);



  public:
    friend std::ostream& operator<< (std::ostream &o, const Level &l);
};


class Universe: public Level
{
  public:
  std::vector<Star> stars;
  public:
  Universe(Level &l);
  //bool play(GLdisplay &d);
  void move();
  bool timeout() {return false;};
  bool won() {return false;};

};

#endif
