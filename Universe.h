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
      vx(0),vy(0),vz(0) {};
    friend std::ostream& operator<< (std::ostream &o, const SkyMass &g);

    inline void operator^ (SkyMass m) {
      register double AX, AY, a1, a2, r3;

      /* Verbindungsvektor */    
      AX = x - m.x;
      AY = y - m.y;

      r3 = hypot(AX,AY);
      r3 = r3*r3*r3;

      /* Einheitsvektor in Verbindungsrichtung */
      AX = AX/r3;
      AY = AY/r3;
      /*  AZ = AZ/r3;*/

      /* Beschleunigung */
      a1 = SUNGRAVTIMEWIDTH*mass;
      a2 = SUNGRAVTIMEWIDTH*m.mass;

      /* Geschwindigkeitsaenderung */
      vx += a1*AX;
      vy += a1*AY;  
      /*  skymass1->vz += a1*AZ;  */
      m.vx -= a2*AX;
      m.vy -= a2*AY;
    };
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
    };

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
  bool move() {return false;};
  bool timeout() {return false;};
  bool won() {return false;};

};

#if 0

class skymass
{
  public:
    double x, y, z; /* in 600000 LJ (0.5 = 300000 LJ = Mitte vom Spielfeld) */
    double vx, vy, vz; /* in m/s */
    double fx, fy, fz; /* Kraft in Newton auf den Koerper */
    double mass; /* in Sonnenmassen */
    int inLevel; /* ob das Objekt durch das Level vorgegeben ist */
    int exists;
    int nograv;
  public:
    static void constructGalaxy(skymass *galaxy, skymass **INstars, int *starsSize);
    skymass(const skymass& p);
    skymass() {};
};

class skygoal
{
  public:
    double x, y, z; /* in 600000 LJ (0.5 = 300000 LJ = Mitte vom Spielfeld) */
    double r; /* Radius */  
};


public:
Universe() {};
//int levelladen(std::ifstream &level);
public:
skygoal goal;
skymass holes[MAX_LEVEL_HOLES+MAX_PUT_HOLES];
skymass galaxies[MAX_GALAXIES];
skymass *stars;
int holesSize;
int galaxiesSize;
int starsSize;
double massreserve;

public:
/* Zeichnet die Oberflaeche zum Setzen der Galaxien */
void drawPut(GLdisplay &display);
/* Zeichnet die Simulation */
void drawSimulation( GLdisplay &display, Kamera *cam, int time ); 
void move(int time);

public:
void eventHorizon();
private:
inline void applyNewton(skymass* skymass1, skymass* skymass2, int teiler)
{
};

#include "local.h"




void constructGalaxy(skymass *galaxy, skymass **INstars, int *starsSize);
#endif









#endif


