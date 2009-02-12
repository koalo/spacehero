#include "Universe.h"

#include "display.h"
#include "GLdisplay.h"
#include "BStatus.h"

#include "displayUniverse.h"

#include "local.h"

Star::Star(Galaxy &g, double R, double phi, double z, double v, double mass)
{
  x = R * cos(phi * M_PI/180) + g.x;
  y = R * sin(phi * M_PI/180) + g.y;
  z = z + g.z;

  vx = v * sin(phi * M_PI/180);
  vy = v * cos(phi * M_PI/180);
  vz = 0;

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
  const double NStars = mass / 4e8;
  const double NOrbits = mass / 4e10;
  const double NStarsPerOrbit = NStars/NOrbits;
  std::vector<Star> accu;

  srand(seed);

  int n=NOrbits;
  double orb = R_MIN_CENTER + (double)((rand() / (RAND_MAX + 1.0)) / 150.0);
  while (n--) {
    orb += R_MIN +  ((double)((rand() / (RAND_MAX + 1.0))) / 150.0);
    double v = sqrt((GRAVKONST * mass * SUNMASS) / (orb * WIDTHINMETERS));
    int m=NStarsPerOrbit;
    while (m--) {
      double phi = (double)(360 * (rand() / (RAND_MAX + 1.0)));
      double z = (double)((1.0/10.0) * (rand() / (RAND_MAX + 1.0)))-(1.0/20.0);
      accu.push_back(Star(*this,orb,phi,z,v));
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


void Universe::move(int time)
{
  int i, j, k, teiler;/* half, full, max;

                         full = ((starsSize+1)*starsSize)/2;
                         half = full/2;

                         if(time % 2)
                         {
                         sum = 0;
                         max = half;
                         } else {
                         sum = half;
                         max = full;
                         }

                         for(i = 0; sum < max; sum+=i++)*/

  time = 5; /* nur Unsinn */

  teiler = 1;
  /*
     if(time%teiler == 0)
     {*/
  for(i = 0; i < starsSize; i++)
  {
    if(stars[i].exists && stars[i].nograv != 1) 
    {
      /*      for(j = i+1; j < starsSize; j++)
              {
              if(stars[j].exists)
              {
              applyNewton(&stars[i], &stars[j], teiler);
              }
              }*/

      for(k = 0; k < holesSize; k++)
      {
        applyNewton(&stars[i], &holes[k], teiler);
      }

      for(j = 0; j < galaxiesSize; j++)
      {
        applyNewton(&galaxies[j], &stars[i], teiler);
      } 
    }
  }

  for(i = 0; i < galaxiesSize; i++)
  {
    for(k = 0; k < holesSize; k++)
    {
      applyNewton(&galaxies[i], &holes[k], teiler);
    }    

    for(k = i+1; k < galaxiesSize; k++)
    {
      if(galaxies[i].exists && galaxies[k].exists && galaxies[i].nograv != 1 && galaxies[k].nograv != 1)
      {
        applyNewton(&galaxies[i], &galaxies[k], teiler);
      }
    }
  }




  /*}*/

  /* Bewegung fuer Sternen */
  for(i = 0; i < starsSize; i++)
  {
    if(stars[i].exists)
    {
      stars[i].x += (stars[i].vx/WIDTHINMETERS)*TIMESCALE;
      stars[i].y += (stars[i].vy/WIDTHINMETERS)*TIMESCALE;
      stars[i].z += (stars[i].vz/WIDTHINMETERS)*TIMESCALE;
    }
  }

  /* Bewegung fuer Koerper in MittelPunkte von Galaxien */
  for(i = 0; i < galaxiesSize; i++)
  {
    galaxies[i].x += (galaxies[i].vx/WIDTHINMETERS)*TIMESCALE;
    galaxies[i].y += (galaxies[i].vy/WIDTHINMETERS)*TIMESCALE;
    galaxies[i].z += (galaxies[i].vz/WIDTHINMETERS)*TIMESCALE;
  }

  /* Bewegung von Black Holes */
  /*for(i = 0; i < holesSize; i++)
    {
    holes[i].x += (holes[i].vx/WIDTHINMETERS)*TIMESCALE;
    holes[i].y += (holes[i].vy/WIDTHINMETERS)*TIMESCALE;
    }*/
}

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
