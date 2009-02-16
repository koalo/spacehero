#include "Universe.h"

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
  goal = Goal(in);
  while(in.good()) {
    char c;
    in >> c;
    if(!in.good()) break;
    std::cerr << "Type: " << c << std::endl;
    switch(c) {
      case 'M': 
        galaxies.push_back( Galaxy(in,true) );
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
    }
  }
  maxtime = 30.0;
}

Goal::Goal(std::ifstream &in)
{
  in >> x >> y >> z;
  in >> radius;
  setlevel();
};

Blackhole::Blackhole(std::ifstream &in) {
  double t;
  in >> x >> y >> z; 
  in >> t >> t >> t; 
  in >> mass; 
  radius = HOLESIZE*sqrt(mass/HOLEMEDIUMMASS);
  setlevel();
};

Galaxy::Galaxy(std::ifstream &in, bool master) :
  master(master),
  lr(true)
{
  in >> x >> y >> z; 
  in >> vx >> vy >> vz; 
  in >> mass; 
  radius = BULGESIZE;
};
