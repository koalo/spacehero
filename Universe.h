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

#include <iostream>
#include <vector>
using namespace std;

class SkyObject
{
  public:
    double x, y, z; /* in 600000 LJ (0.5 = 300000 LJ = Mitte vom Spielfeld) */
    double radius;

  protected:
    bool exists;
    bool level;

  public:
    SkyObject();
    SkyObject(double ix, double iy, double iradius);
    virtual ~SkyObject() {};
    void setlevel();
    bool setexists(bool b=true);
    bool getexists();
    bool inLevel();
    friend ostream& operator<< (ostream &o, const SkyObject &g);
};

class SkyMass : public SkyObject
{
  public:
    double mass; /* in Sonnenmassen */
    double vx, vy, vz; /* in m/s */

  public:
    SkyMass();
    SkyMass(double ix, double iy, double imass, double iradius);
    
    inline void newton(SkyMass &m, double &delta);
    inline void move(double delta);

    friend ostream& operator<< (ostream &o, const SkyMass &g);
};

class Goal : public SkyObject 
{
  public:
    Goal();
    Goal(ifstream &in);
  public:
    friend ostream& operator<< (ostream &o, const Goal &g);
};

class Blackhole : public SkyMass
{
  public:
    Blackhole(ifstream &in);
    Blackhole(double ix, double iy, double imass);
  public:
    friend ostream& operator<< (ostream &o, const Blackhole &g);
};

class Star;

class Galaxy : public SkyMass
{
  protected:
    bool master;

  public:
    bool lr; // links true /rechts

  private:
    static const double R_MIN_CENTER = 0.02;
    static const double R_MIN = 0.0005;

  public:
    Galaxy(ifstream &in);
    Galaxy(double ix, double iy, double imass, bool imaster, bool ilr);

    vector<Star> getStars(int seed);
    bool getmaster() { return master; };

  public:
    friend ostream& operator<< (ostream &o, const Galaxy &g);
};

class Star : public SkyMass
{
  public:
    Star(Galaxy &g,double R, double phi, double z, double v, double mass=1e3);
  public:
    friend ostream& operator<< (ostream &o, const Star &g);
};

class Sky 
{
  protected:
    int seed;
  public:
    vector<Star> stars;
    vector<Galaxy> galaxies;
    Sky();
    virtual ~Sky() {};
    void calcStars();
};

class Level : public Sky
{
  protected:
    unsigned int t0;
    double maxtime;
    double lastt;
    double m_delta;

    double m_fpst;
    double m_fps;
    string name;
    string filename;
  public:
    vector<Blackhole> holes;
    Goal goal;
  public:
    Level(ifstream &in);
    Level();
    virtual ~Level() {};
    void setName(string name);
    void setFilename(string name);
    string getName();
    string getFilename();

  public:
    void tinit(); // start time measure
    double elapsed(); // get elapsed time
    void tick();
    void tack(double weight=0.9); // measure round time
    double ldelta(double weight=0.1 );
    double delta(); // filtered delta
    double getmaxtime() const; // get maxtime
    bool timeout(); // TODO: fix to use summed movement time
    double fps();

  public:
    friend ostream& operator<< (ostream &o, const Level &l);
};

namespace Error {
  class ParseLevel {
    const char *p;
    public:
    ParseLevel(const char* q): p(q) {}
    const char* msg() {return p;}
  };
}

class Universe: public Level
{
  protected:
    bool m_won;
    bool stargrav;
  public:
    Universe(Level &l);
    Universe();

    void move(double delta);
    bool won();
    void eventHorizon();
    void setStargrav(bool grav);
};

#endif
