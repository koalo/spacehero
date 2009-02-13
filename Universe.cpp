#include "Universe.h"

#include "display.h"
#include "GLdisplay.h"
#include "BStatus.h"

#include "displayUniverse.h"

#include "local.h"

Star::Star(Galaxy &g, double R, double phi, double z, double v, double mass)
{
  x=y=z=vx=vy=vz=0;
  x = R * cos(phi * M_PI/180) + g.x;
  y = R * sin(phi * M_PI/180) + g.y;
  z = z + g.z;

  vx = v * cos((phi-90-(g.lr?0:180)) * M_PI/180) + g.vx;
  vy = v * sin((phi-90-(g.lr?0:180)) * M_PI/180) + g.vy;
  vz = 0+g.vz;

  this->mass = mass;
}


Level::Level(std::ifstream &in)
{
  goal = Goal(in);
  while(in.good()) {
    char c;
    in >> c;
    std::cerr << "Type: " << c << std::endl;
    switch(c) {
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
}

std::vector<Star> Galaxy::getStars(int seed) {
  double NStars = mass / 4e8;
  double NOrbits = mass / 4e10;
  double NStarsPerOrbit = NStars/NOrbits;
  std::vector<Star> accu;

  srand(seed);

  int n=NOrbits;
  double orb = R_MIN_CENTER + (double)((rand() / (RAND_MAX + 1.0)) / 150.0);
  orb *= 10;
  while (n--) {
    orb += R_MIN + 10.0* ((double)((rand() / (RAND_MAX + 1.0))) / 550.0);
    double v = sqrt((GRAVKONST * mass * SUNMASS) / (orb * WIDTHINMETERS));
    int m=NStarsPerOrbit;
    while (m--) {
      double phi = (double)(360 * (rand() / (RAND_MAX + 1.0)));
      double z = (double)((1.0/10.0) * (rand() / (RAND_MAX + 1.0)))-(1.0/20.0);
      accu.push_back(Star(*this,orb,phi,z,v,1e3));
    }
  }
  return accu;
}

Universe::Universe(Level &l) :
  Level(l)
{
  for(std::vector<Galaxy>::iterator i = galaxies.begin(); i != galaxies.end(); i++) {
    std::vector<Star> gstars = i->getStars(seed);
    std::cerr << "got " << gstars.size() << " stars" << std::endl;
    copy(gstars.begin(),gstars.end(),back_inserter(stars));
  }
  std::cerr << "universe has " << stars.size() << " stars" << std::endl;
}


void Universe::move()
{
// star: hole, galaxy
  for(std::vector<Star>::iterator i = stars.begin(); i!= stars.end(); i++) {
    for(std::vector<Galaxy>::iterator j = galaxies.begin(); j!= galaxies.end(); j++) {
      *i ^ *j;
    }
    for(std::vector<Blackhole>::iterator k = holes.begin(); k!= holes.end(); k++) {
      *i ^ *k;
    }
  }
// galaxy: hole, galaxy
  for(std::vector<Galaxy>::iterator i = galaxies.begin(); i!= galaxies.end(); i++) {
    for(std::vector<Galaxy>::iterator j = galaxies.begin(); j!= galaxies.end(); j++) {
      if(i!=j) *i ^ *j;
    }
    for(std::vector<Blackhole>::iterator k = holes.begin(); k!= holes.end(); k++) {
      *i ^ *k;
    }
  }

  for(std::vector<Star>::iterator i = stars.begin(); i!= stars.end(); i++) {
    i->move();
  }

  for(std::vector<Galaxy>::iterator i = galaxies.begin(); i!= galaxies.end(); i++) {
    i->move();
  }
}

#if 0




skymass::skymass(const skymass& p):
  x(p.x),y(p.y),z(p.z),
  vx(p.vx),
  vy(p.vy),
  vz(p.vz),
  fx(p.fx),
  fy(p.fy),
  fz(p.fz),
  mass(p.mass),
  inLevel(p.inLevel),
  exists(p.exists),
  nograv(p.nograv)
{}


void Universe::eventHorizon()
{
  /* fuer jede Stern wird geprueft ob die sich zu nah zu Black Hole befindet */
  /* wenn der Abstand kleiner als r ist, dann soll die verschwinden -> stars[j].exist = 0 statt 1 */ 

  int i, j;
  double r, x, y;


  for(i = 0; i < holesSize; i++)
  {
    for(j = 0; j < starsSize; j++)
    {
      x = holes[i].x - stars[j].x;
      y = holes[i].y - stars[j].y;
      r = sqrt(x*x + y*y);
      if(r < HOLESIZE*0.5*sqrt(holes[i].mass/HOLEMEDIUMMASS))
      {
        stars[j].exists = 0;
      }
    }
    /*fuer MittelPunkten von Galxies auch */
    for(j = 0; j < galaxiesSize; j++)
    {
      x = holes[i].x - galaxies[j].x;
      y = holes[i].y - galaxies[j].y;
      r = sqrt(x*x + y*y);
      if(r < HOLESIZE*0.5*sqrt(holes[i].mass/HOLEMEDIUMMASS))
      {
        galaxies[j].exists = 0;
      }
    }

  }
}
#endif
