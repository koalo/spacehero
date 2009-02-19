#include "Universe.h"
#include <fstream>

Level::Level(std::ifstream &in) :
  t0(),
  maxtime(30.0),
  lastt(0),
  m_delta(0),
  m_fpst(0),
  m_fps(0),
  holes(),
  galaxies(),
  goal(),
  seed(0)

{
  char c; int i;
  in.exceptions(std::ios_base::badbit|std::ios_base::failbit|std::ios_base::eofbit);
  in >> c >> i;
  if(c!='V' && i!=1) throw Error::ParseLevel("wrong version in level file");
  while( in >> c, in.good()) {
    switch(c) {
      case 'D': 
        goal = Goal(in);
        break;
      case 'G': 
        galaxies.push_back( Galaxy(in) );
        break;
      case 'H':
        holes.push_back( Blackhole(in) );
        break;
      case 'S':
        in >> seed;
        break;
      case 'T':
        in >> maxtime;
        break;
      default:
        throw Error::ParseLevel("funny data in level");
    }
  }
}

Goal::Goal(std::ifstream &in)
{
  bool P=false,R=false;
  char c;
  while( in >> c, in.good()) {
    switch(c) {
      case 'P': P=true;
                in >> x >> y >> z;
                break;
      case 'R': R=true;
                in >> radius;
      default:
                throw Error::ParseLevel("funny data in level");
    }
  }
  if(!P) throw Error::ParseLevel("Missing Position for Goal");
  if(!R) throw Error::ParseLevel("Missing Radius for Goal");
  setlevel();
};

Blackhole::Blackhole(std::ifstream &in) {
  bool P=false,W=false;
  char c;
  while( in >> c, in.good()) {
    switch(c) {
      case 'P': 
        in >> x >> y >> z;
        break;
      case 'V':
        double t;
        in >> t >> t >> t; 

      case 'W':
        in >> mass;
      default:
        throw Error::ParseLevel("funny data in level");
    }
  }
  if(!P) throw Error::ParseLevel("Missing Position for Hole");
  if(!W) throw Error::ParseLevel("Missing Weight for Hole");
  radius = HOLESIZE*sqrt(mass/HOLEMEDIUMMASS);
  setlevel();
};

Galaxy::Galaxy(std::ifstream &in, bool master) :
  master(master),
  lr(true)
{
  bool P=false,V=false,W=false;
  char c;
  while( in >> c, in.good()) {
    switch(c) {
      case 'P': 
        in >> x >> y >> z;
        break;
      case 'V':
        in >> vx >> vy >> vz; 
        break;
      case 'M':
        master=true;
        break;
      case 'W':
        in >> mass;
      default:
        throw Error::ParseLevel("funny data in level");
    }
  }
  if(!P) throw Error::ParseLevel("Missing Position for Galaxy");
  if(!V) throw Error::ParseLevel("Missing Velocity for Galaxy");
  if(!W) throw Error::ParseLevel("Missing Weight for Galaxy");
  setlevel();
  radius = BULGESIZE;
};
