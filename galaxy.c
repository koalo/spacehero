#include "galaxy.h"

#ifdef BETTER
#ifdef FAST
static inline 
#endif
void applyNewton(skymass* skymass1, skymass* skymass2, int teiler)
{
  #ifdef FAST
    register double AX, AY, a1, a2, r3;
  #else
    double AX, AY, a1, a2, r3;
  #endif

  /* Verbindungsvektor */    
  AX = skymass2->x - skymass1->x;
  AY = skymass2->y - skymass1->y;
/*  AZ = skymass2->z - skymass1->z;*/

  /* Quadrat des Abstands */
  /*r3 = pow(r3,1.5);*/
/*
  double w,walt=0;
  r3 = (AX*AX) + (AY*AY);
  w = r3/4;
  while(w!=walt)
  {
    walt=w;
    w = (w+(r3/w))/2;
  }
  r3 = w;
  r3 = r3*r3*r3;
*/


  /* 2  norm */

/*
  r3 = (AX*AX) + (AY*AY);
  r3 = sqrt(r3);
  r3 = r3*r3*r3;
*/

  r3 = hypot(AX,AY);
  r3 = r3*r3*r3;


  /* 1 norm */
  /*
  r3 = abs(AX)+abs(AY);
  */
  /* inf norm */
  /*
  #define max(a,b) (a)>(b)?(a):(b)
  r3 = max(AX,AY);
  */
  
  /* Einheitsvektor in Verbindungsrichtung */
  AX = AX/r3;
  AY = AY/r3;
/*  AZ = AZ/r3;*/

  /* Beschleunigung */
  a1 = SUNGRAVTIMEWIDTH*teiler*skymass2->mass;
  a2 = SUNGRAVTIMEWIDTH*teiler*skymass1->mass;

  /* Geschwindigkeitsaenderung */
  skymass1->vx += a1*AX;
  skymass1->vy += a1*AY;  
/*  skymass1->vz += a1*AZ;  */
  skymass2->vx -= a2*AX;
  skymass2->vy -= a2*AY;
/*  skymass2->vz -= a2*AZ;*/
/*  ((SUNGRAVTIMEWIDTH/r2)*skmass)*(AX/r)
  = (SUNGRAVTIMEWIDTH*skymass*AX)/r3*/
}

#else
#ifdef FAST
static inline 
#endif
void applyNewton(skymass* skymass1, skymass* skymass2)
{
  #ifdef FAST
    register double AX, AY, eX, eY, a1, a2, r2;
  #else
    double AX, AY, eX, eY, a1, a2, r2;
  #endif

  /* Verbindungsvektor */    
  AX = skymass2->x - skymass1->x;
  AY = skymass2->y - skymass1->y;

  /* Quadrat des Abstands */
  r2 = (AX*AX) + (AY*AY);

  /* Einheitsvektor in Verbindungsrichtung */
  eX = AX/sqrt(r2);
  eY = AY/sqrt(r2);

  /* Abstand in Metern */
  r2 = r2*WIDTHINMETERS*WIDTHINMETERS;

  /* Beschleunigung */
  a1 = GRAVKONST*(((skymass2->mass*SUNMASS))/(r2));
  a2 = GRAVKONST*(((skymass1->mass*SUNMASS))/(r2));
   
  /* Geschwindigkeitsaenderung */
  skymass1->vx += a1*eX*TIMESCALE;
  skymass1->vy += a1*eY*TIMESCALE;  
  skymass2->vx -= a2*eX*TIMESCALE;
  skymass2->vy -= a2*eY*TIMESCALE;
}
#endif

void eventHorizon(skymass stars[], int starsSize, skymass holes[], int holesSize, skymass galaxies[], int galaxiesSize)
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

void move(skymass galaxies[], int galaxiesSize, skymass holes[], int holesSize, skymass stars[], int starsSize, int time)
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

void constructGalaxy(skymass *galaxy, skymass **INstars, int *starsSize)
{
  skymass star;
  double Stars_Angle;
  double R_Array[ORBITS_MAX],
         Orbit_Velocity[ORBITS_MAX], first_angle;
         
  int orbits1, i , j, s, Stars_Amount[ORBITS_MAX], starsAmount, starsAmountOrbit, realstarsAmount;
  skymass *stars;

/* Zufaellige Mittelpunkt fuer Galaxy festlegen */
  /*center.x = (double)((1.0/12) + ((3.0/12) * (rand() / (RAND_MAX + 1.0))));
  center.y = (double)((1.0/12) + ((10.0/12) * (rand() / (RAND_MAX + 1.0))));
  *galaxies = center;*/

/* Masse für jede Stern verteilen*/

/* Anzahl von Orbits, von Masse abhaengig (Orbits MIN = 5, MAX = 15) */
  orbits1 = (galaxy->mass * 2) / 4e10;
  if(orbits1 > ORBITS_MAX)
  {
    printf("Galaxie ist zu schwer!\n");
    exit(1);
  }
  starsAmount = (galaxy->mass) / 4e8;

/* R_Array[20] <- Array von Radius der Orbit fuer 2 Galaxien */
/* 1 Radius zufaellige Radius berechnen (incl. R_MIN_CENTER) */
  R_Array[0] = R_MIN_CENTER + (double)((rand() / (RAND_MAX + 1.0)) / 150.0);

/* Naechste Radius Berechnen (incl. R_MIN) */

  for (i = 1; i < orbits1; i++)
  {
    R_Array[i] = R_Array[i-1] + R_MIN +  ((double)((rand() / (RAND_MAX + 1.0))) / 550.0);
  }


/*  starsAmountOrbit = starsAmount / orbits1;*/
/* das kann weg, brauch ich Stars_amount[i] nicht mehr*/
  realstarsAmount = 0;
  for(i = 0; i < orbits1; i++)
  {  
    Stars_Amount[i] = starsAmount / (orbits1);
    realstarsAmount += Stars_Amount[i];
  }

  starsAmountOrbit = starsAmount / orbits1;
 
/* starsArrayGroesse festlegen */
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

  *INstars = (skymass*)realloc(*INstars, sizeof(skymass) * (realstarsAmount + *starsSize));
  if(*INstars == NULL)
  {
    printf("Kein Arbeitsspeicher fuer die Sterne!\n");
    exit(1);
  }

  stars = &(*INstars)[*starsSize];

  /* X, Y Koordiante fuer jede Stern ausrechnen */

/* Erste Winkel (first_angle) zwischen y=0,+x und Erste Stern auf der Bahn zufaellig,
 * dann jede naechste Wikel durch Drewinkel Stars_Angle erzeugt
 * Erzeugung durch Drehmatrix:
 * [x2] = (cos&  -sin&) [x1]
 * [y2]   (sin&   cos&) [y1]
 *
 * dazu star.vx, star.vy <- normierte GeschwindigkeitsVektoren von Stern senkrecht zu R, entgegengesetzt zu Zeit
 */

/* fuer Galaxy1 */

  s = -1;
  for(i = 0; i < orbits1; i++)
  {
    s++;

    first_angle = (double)(360 * (rand() / (RAND_MAX + 1.0)));

    star.x = (R_Array[i] * cos(first_angle * M_PI/180)) - (0.0 * sin(first_angle * M_PI/180)) + galaxy->x;
    star.y = (R_Array[i] * sin(first_angle * M_PI/180)) + (0.0 * cos(first_angle * M_PI/180)) + galaxy->y;


    star.vx = (0 * cos(first_angle * M_PI/180)) - (1.0 * sin(first_angle * M_PI/180));
    star.vy = (0 * sin(first_angle * M_PI/180)) + (1.0 * cos(first_angle * M_PI/180));

    stars[s] = star;

    for(j = 1; j < (starsAmountOrbit); j++)
    {
      s++;

      Stars_Angle = (double)(360 * (rand() / (RAND_MAX + 1.0)));

      star.x = (stars[s-1].x - galaxy->x) * cos(Stars_Angle * M_PI/180) - (stars[s-1].y - galaxy->y) * sin(Stars_Angle * M_PI/180) + galaxy->x;

      star.y = (stars[s-1].x - galaxy->x) * sin(Stars_Angle * M_PI/180) + (stars[s-1].y - galaxy->y) * cos(Stars_Angle * M_PI/180) + galaxy->y;

      star.vx = (stars[s-1].vx * cos(Stars_Angle * M_PI/180)) - (stars[s-1].vy * sin(Stars_Angle * M_PI/180));
      star.vy = (stars[s-1].vx * sin(Stars_Angle * M_PI/180)) + (stars[s-1].vy * cos(Stars_Angle * M_PI/180));

      stars[s] = star;
    }

  }

  starsAmount = s + 1;
  *starsSize += starsAmount;

/* Z Koordinate fuer jede Stern ausgeben */
/* fuer 4 Gruppen Teilen */
/* In der Mitte soll dichter sein ale am Randen */

/* 1 Gruppe kriegt Z Koordinate von 0 bis 1/40 */
  for(i = 0; i < starsAmount; i=i+4)
  {
    stars[i].z = (double)((rand() / (RAND_MAX + 1.0)) / 100);
  }

/* von 0 bis -1/40 */
  for(i = 1; i < starsAmount; i=i+4)
  {
    stars[i].z = - (double)((rand() / (RAND_MAX + 1.0)) / 100);
  }

/* von 0 bis 1/30 */
  for(i = 2; i < starsAmount; i=i+4)
  {
    stars[i].z = (double)((rand() / (RAND_MAX + 1.0)) / 80);
  }

/* von 0 bis -1/30 */
  for(i = 3; i < starsAmount; i=i+4)
  {
    stars[i].z = - (double)((rand() / (RAND_MAX + 1.0)) / 80);
  }

/* Masse für jede Stern verteilen*/

/* Massen fur Sternen die herum fliegen */
  for(i = 0; i < starsAmount; i++)
  {
    stars[i].mass = 1e3;
    stars[i].exists = 1;
  }

/* Nuetige AnfangsGeschwindigkeit 
 * Orbit_Velocity = sqrt((G*M)/R)
 *
 * und NewtonBeschleunigung von Sternen auf verschidenen Bahnen
 * Orbit_Accelerate = G*(M/R²)
 *
 * M - Masse von Stern in Mittelpunkt
 *GRAVKONST- Gravitationskonstante
 */
  
  for(i=0; i < orbits1; i++)
  {
    Orbit_Velocity[i] = sqrt((GRAVKONST * galaxy->mass * SUNMASS) / (R_Array[i] * WIDTHINMETERS));
  }



/* VX und VY -Geschwindigkeiten, für jede Stern berechnen */

  s = -1;
  for(i = 0; i < orbits1; i++)
  { 

    for(j = 0; j < (starsAmountOrbit); j++)
    {
      s++;

/* Normirte GeschwindigkeitsVektor * Orbit_Geschwindigkeit */
      stars[s].vx *= Orbit_Velocity[i];
      stars[s].vy *= Orbit_Velocity[i];
    
/* Dazu noch AnfangsGeschwindigkeit von Galaxy */
      stars[s].vx += galaxy->vx;
      stars[s].vy += galaxy->vy;

    }

  }
}
