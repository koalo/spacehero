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
#include "Universe.h"

#include "GLdisplay.h"
#include "local.h"

Star::Star(Galaxy &g, double R, double phi, double iz, double v, double mass)
{
  double sx,sy,sz,svx,svy,svz;
  //x=y=z=vx=vy=vz=0;
  sx = R * cos(phi * M_PI/180);
  sy = R * sin(phi * M_PI/180);
  sz = iz;

  double tilt = 0.01; //0.25*M_PI;

  x = sx;
  y = sy * cos(tilt) - sz * sin(tilt);
  z = sy * sin(tilt) + sz * cos(tilt);

  phi += -90-(g.lr?0:180);
  svx = 1e0 * v * cos(phi * M_PI/180);
  svy = 1e0 * v * sin(phi * M_PI/180);
  svz = 0;

  vx = svx;
  vy = svy * cos(tilt) - svz * sin(tilt);
  vz = svy * sin(tilt) + svz * cos(tilt);

  x += g.x;
  y += g.y;
  z += g.z;

  vx += g.vx;
  vy += g.vy;
  vz += g.vz;

  this->mass = mass;
  radius = STARSIZE;
}

std::vector<Star> Galaxy::getStars(int seed) {
  double v;
  double NStars = mass / 5e8;
  double NOrbits = mass / 2e10;
  double NStarsPerOrbit = NStars/NOrbits;
  std::vector<Star> accu;

  srand(seed);

  int n=NOrbits;
  double orb = R_MIN_CENTER + (double)((rand() / (RAND_MAX + 1.0)) / 150.0);
  while (n--) 
  {
    orb += R_MIN + ((double)((rand() / (RAND_MAX + 1.0))) / 550.0);
    int m=NStarsPerOrbit;
    while (m--)
    {
      double phi = (double)(360 * (rand() / (RAND_MAX + 1.0)));
      double z = (double)((rand() / (RAND_MAX + 1.0)) - 0.5)*(0.001/orb);
      z = 0;
      v = sqrt((GRAVKONST * mass * SUNMASS) / (hypot(orb,z) * WIDTHINMETERS));
      accu.push_back(Star(*this,orb,phi,z,v,1e3));
    }
  }
  return accu;
}

Universe::Universe(Level &l) :
  Level(l),
  m_won(false),
  stargrav(false)
{
  calcStars();
}


void Sky::calcStars()
{
  stars.clear();

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
    for(std::vector<Blackhole>::iterator k = holes.begin(); k!= holes.end(); k++) {
      i->newton(*k,delta);
    }
    if(stargrav)
    {
      //for(std::vector<Star>::iterator l = stars.begin(); l!= stars.end(); l++) {
       // if(i!=l) i->newton(*l,delta);
      ////}
      //for(unsigned int a = 0; a < stars.size(); a++){}
    }
    i->move(delta);
  }
  // galaxy: hole, galaxy
  for(std::vector<Galaxy>::iterator i = galaxies.begin(); i!= galaxies.end(); i++) {
    for(std::vector<Galaxy>::iterator j = i+1; j!= galaxies.end(); j++) {
      if(i!=j) i->newton(*j,delta);
    }
    for(std::vector<Blackhole>::iterator k = holes.begin(); k!= holes.end(); k++) {
      i->newton(*k,delta);
    }
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

bool Universe::won()
{
  int master = 0;
  int ingoal = 0;
  for(std::vector<Galaxy>::iterator i = galaxies.begin(); i!= galaxies.end(); i++) 
  {
    if( i->getmaster() )
    {
      master++;
      if( hypot(i->x - goal.x,i->y - goal.y) < goal.radius && i->exists && goal.getexists() )
      { 
        ingoal++;
      }
    }
  }
  return (m_won = (ingoal == master && master > 0));
}
