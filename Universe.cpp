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

#include <math.h>

#include "GLdisplay.h"
#include "Constants.h"

SkyObject::SkyObject()
  : x(0), y(0), z(0), radius(0), exists(true), level(false)
{
}

SkyObject::SkyObject(double ix, double iy, double iradius) 
  : x(ix), y(iy), z(0), radius(iradius), exists(true), level(false)
{
}

void SkyObject::setlevel()
{
  level = true;
}

bool SkyObject::setexists(bool b)
{
  return exists = b;
}

bool SkyObject::getexists()
{
  return exists; 
}

bool SkyObject::inLevel()
{
  return level;
}

SkyMass::SkyMass()
  : SkyObject(), mass(0), vx(0), vy(0), vz(0)
{
}

SkyMass::SkyMass(double ix, double iy, double imass, double iradius)
  : SkyObject(ix, iy, iradius), mass(imass), vx(0), vy(0), vz(0)
{
}

Goal::Goal()
  : SkyObject()
{
}

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

vector<Star> Galaxy::getStars(int seed) {
  double v;
  double NStars = mass / 5e8;
  double NOrbits = mass / 2e10;
  double NStarsPerOrbit = NStars/NOrbits;
  vector<Star> accu;

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

Sky::Sky() 
  : seed(0), stars(), galaxies()
{
}

Universe::Universe(Level &l) :
  Level(l),
  m_won(false),
  stargrav(false)
{
  calcStars();
}

Level::Level()
  : t0(), maxtime(30.0), lastt(0), m_delta(0), m_fpst(0), m_fps(0), name(""), filename(""), holes(), goal()
{
}

void Level::setName(string name)
{
  this->name = name;

  unsigned int pos = -1;
  pos = name.find(" ",pos+1);

  while(pos < name.length()-1)
  {
    name = name.replace(pos, 1, "_");
    pos = name.find(" ",pos+1);
  }
  
  this->filename = name;
}

void Level::setFilename(string name)
{
  this->filename = name;

  unsigned int pos = -1;
  pos = name.find("_",pos+1);

  while(pos < name.length()-1)
  {
    name = name.replace(pos, 1, " ");
    pos = name.find("_",pos+1);
  }
  
  this->name = name;
}

string Level::getFilename()
{
  return filename;
}

string Level::getName()
{
  return name;
}

void Level::tick()
{
  lastt=elapsed(); 
}

void Level::tack(double weight)
{
  m_delta = weight*m_delta + (1-weight)*(elapsed()-lastt);
}

double Level::ldelta(double weight)
{ 
  return weight*m_delta + (1-weight)*(elapsed()-lastt);
}

double Level::delta()
{
  return m_delta;
}

double Level::getmaxtime() const 
{
  return maxtime; 
}

bool Level::timeout()
{
  return elapsed() > maxtime; 
}

double Level::fps()
{
  return 1/m_delta;
}

void Level::tinit()
{
  t0 = SDL_GetTicks();
}

double Level::elapsed() 
{
  return (SDL_GetTicks()-t0)/1000.0; 
}

Blackhole::Blackhole(double ix, double iy, double imass) 
  : SkyMass(ix, iy, imass, HOLESIZE*sqrt(imass/HOLEMEDIUMMASS))
{
}

Galaxy::Galaxy(double ix, double iy, double imass, bool imaster, bool ilr) 
  : SkyMass(ix, iy, imass, BULGESIZE), master(imaster), lr(ilr)
{
}

inline void SkyMass::move(double delta) {
  x += (vx/WIDTHINMETERS)*TIMESCALE*delta;
  y += (vy/WIDTHINMETERS)*TIMESCALE*delta;
  z += (vz/WIDTHINMETERS)*TIMESCALE*delta;
  //if(hypot(vx,vy)>1) cerr << ".";
}

inline void SkyMass::newton(SkyMass &m, double &delta) {
  double AX, AY, AZ, a1, a2, r3;
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
}

void Sky::calcStars()
{
  stars.clear();

  for(vector<Galaxy>::iterator i = galaxies.begin(); i != galaxies.end(); i++) {
    vector<Star> gstars = i->getStars(seed);
    cerr << "got " << gstars.size() << " stars" << endl;
    copy(gstars.begin(),gstars.end(),back_inserter(stars));
  }
  cerr << "universe has " << stars.size() << " stars" << endl;
}

Universe::Universe()
  : m_won(false),stargrav(false)
{
}

void Universe::setStargrav(bool grav)
{
  this->stargrav = grav;
}

void Universe::move(double delta)
{
  delta *= 50; // speedup
  // star: hole, galaxy
  for(vector<Star>::iterator i = stars.begin(); i!= stars.end(); i++) {
    for(vector<Galaxy>::iterator j = galaxies.begin(); j!= galaxies.end(); j++) {
      i->newton(*j,delta);
    }
    for(vector<Blackhole>::iterator k = holes.begin(); k!= holes.end(); k++) {
      i->newton(*k,delta);
    }
    if(stargrav)
    {
      //for(vector<Star>::iterator l = stars.begin(); l!= stars.end(); l++) {
       // if(i!=l) i->newton(*l,delta);
      ////}
      //for(unsigned int a = 0; a < stars.size(); a++){}
    }
    i->move(delta);
  }
  // galaxy: hole, galaxy
  for(vector<Galaxy>::iterator i = galaxies.begin(); i!= galaxies.end(); i++) {
    for(vector<Galaxy>::iterator j = i+1; j!= galaxies.end(); j++) {
      if(i!=j) i->newton(*j,delta);
    }
    for(vector<Blackhole>::iterator k = holes.begin(); k!= holes.end(); k++) {
      i->newton(*k,delta);
    }
    i->move(delta);
  }
  
  eventHorizon();
}

#define XMALL 1e-250

void Universe::eventHorizon()
{
  // fuer jede Stern wird geprueft ob die sich zu nah zu Black Hole befindet
  // wenn der Abstand kleiner als r ist, dann soll die verschwinden -> stars[j].exist = 0 statt 1 
  for(vector<Blackhole>::iterator i = holes.begin(); i!= holes.end(); i++) {
    for(vector<Star>::iterator j = stars.begin(); j!= stars.end(); j++) {
      double r = hypot(i->x - j->x,i->y - j->y);
        if(r < HOLESIZE*0.5*sqrt(i->mass/HOLEMEDIUMMASS))
          j->setexists(false);
    }
    // fuer MittelPunkten von Galxies auch
    for(vector<Galaxy>::iterator j = galaxies.begin(); j!= galaxies.end(); j++)
    {
      double r = hypot(i->x - j->x,i->y - j->y);
      if(r < HOLESIZE*0.5*sqrt(i->mass/HOLEMEDIUMMASS))
      {
	j->setexists(false);
	if(abs(j->vx) > XMALL*100 && abs(j->vy) > XMALL*100)
	{
	  printf("%e\n",j->vx);
	  j->vx *= XMALL;
	  j->vy *= XMALL;
	}
      }
    }
  }
}

bool Universe::won()
{
  int master = 0;
  int ingoal = 0;
  for(vector<Galaxy>::iterator i = galaxies.begin(); i!= galaxies.end(); i++) 
  {
    if( i->getmaster() )
    {
      master++;
      if( hypot(i->x - goal.x,i->y - goal.y) < goal.radius && i->getexists() && goal.getexists() )
      { 
	i->vx /= 1e16;
	i->vy /= 1e16;
        ingoal++;
      }
    }
  }
  return (m_won = (ingoal == master && master > 0));
}
