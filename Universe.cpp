#include "Universe.h"

#include "display.h"
#include "GLdisplay.h"
#include "BStatus.h"

#include "displayUniverse.h"

#include "local.h"

Star::Star(Galaxy &g, double R, double phi, double z, double v, double mass)
{
  //x=y=z=vx=vy=vz=0;
  x = R * cos(phi * M_PI/180) + g.x;
  y = R * sin(phi * M_PI/180) + g.y;
  z = z + g.z;

  phi += -90-(g.lr?0:180);
  vx = 1e0 * v * cos(phi * M_PI/180) + g.vx;
  vy = 1e0 * v * sin(phi * M_PI/180) + g.vy;
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

std::vector<Star> Galaxy::getStars(int seed) {
  double NStars = mass / 4e8;
  double NOrbits = mass / 2e10;
  double NStarsPerOrbit = NStars/NOrbits;
  std::vector<Star> accu;

  srand(seed);

  int n=NOrbits;
  double orb = R_MIN_CENTER + (double)((rand() / (RAND_MAX + 1.0)) / 150.0);
  while (n--) {
    orb += R_MIN + ((double)((rand() / (RAND_MAX + 1.0))) / 550.0);
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


void Universe::move(double delta)
{
  delta *= 30; // speedup
  // star: hole, galaxy
  for(std::vector<Star>::iterator i = stars.begin(); i!= stars.end(); i++) {
    for(std::vector<Galaxy>::iterator j = galaxies.begin(); j!= galaxies.end(); j++) {
      i->newton(*j,delta);
    }
    for(std::vector<Galaxy>::iterator j = galaxies.begin(); j!= galaxies.end(); j++) {
      i->newton(*j,delta);
    }
    for(std::vector<Blackhole>::iterator k = holes.begin(); k!= holes.end(); k++) {
      i->newton(*k,delta);
    }
//    for(std::vector<Star>::iterator l = stars.begin(); l!= stars.end(); l++) {
//      if(i!=l) i->newton(*l,delta);
//    }
  }
  // galaxy: hole, galaxy
  for(std::vector<Galaxy>::iterator i = galaxies.begin(); i!= galaxies.end(); i++) {
    for(std::vector<Galaxy>::iterator j = i+1; j!= galaxies.end(); j++) {
      if(i!=j) i->newton(*j,delta);
    }
    for(std::vector<Blackhole>::iterator k = holes.begin(); k!= holes.end(); k++) {
      i->newton(*k,delta);
    }
  }

  for(std::vector<Star>::iterator i = stars.begin(); i!= stars.end(); i++) {
    i->move(delta);
  }

  for(std::vector<Galaxy>::iterator i = galaxies.begin(); i!= galaxies.end(); i++) {
    i->move(delta);
  }
  eventHorizon();
}

void Universe::eventHorizon()
{
  // fuer jede Stern wird geprueft ob die sich zu nah zu Black Hole befindet
  // wenn der Abstand kleiner als r ist, dann soll die verschwinden -> stars[j].exist = 0 statt 1 
  for(std::vector<Blackhole>::iterator i = holes.begin(); i!= holes.end(); i++) {
    for(std::vector<Star>::iterator j = stars.begin(); j!= stars.end(); j++) {
      double r = hypot(i->x - j->x,i->y - j->y);
        if(r < HOLESIZE*0.5*sqrt(i->mass/HOLEMEDIUMMASS))
          j->setexists(false);
    }
    // fuer MittelPunkten von Galxies auch
    for(std::vector<Galaxy>::iterator j = galaxies.begin(); j!= galaxies.end(); j++) {
      double r = hypot(i->x - j->x,i->y - j->y);
        if(r < HOLESIZE*0.5*sqrt(i->mass/HOLEMEDIUMMASS))
          j->setexists(false);
    }

  }
}

bool Universe::won() {
  for(std::vector<Galaxy>::iterator i = galaxies.begin(); i!= galaxies.end(); i++) {
    if( i->getmaster() )
      if( hypot(i->x - goal.x,i->y - goal.y) < goal.radius ) 
        m_won = true;
  }
  return m_won;
}
