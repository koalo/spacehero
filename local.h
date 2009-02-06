/** @file local.h
 * @brief Globale Konstanten und Strukturen.
 */

#ifndef LOCAL_H
#define LOCAL_H

#define YEARINSECONDS 365*24*60*60
#define TIMESCALE 1400000*YEARINSECONDS /* -> so viele Sekunden werden pro Schritt berechnet */
#define TIMESTEP 0.02 /* Sekunden zwischen zwei Berechnungen */

#define GRAVKONST 6.67428e-11
#define SUNMASS 1.9891e30
#define LJ 9.461e15
#define WIDTHINMETERS (600000*LJ)
/*#define WIDTHINMETERS 56766e17*/
#define WIDTHINMETERS2 3222378756e34

#define SUNGRAV 13275810348e10
/*#define SUNGRAV (GRAVKONST*SUNMASS)*/
#define SUNGRAVTIME 5861323371883392e18
/*#define SUNGRAVTIMEWIDTH (SUNGRAVTIME/WIDTHINMETERS2)*/
#define SUNGRAVTIMEWIDTH 1.81894303e-10

/* maximale Anzahl an... */ 
#define MAX_GALAXIES 50
#define MAX_PUT_HOLES 10
#define MAX_LEVEL_HOLES 50
#define MAX_STARS 2000

#define HOLESMALLMASS 8e11
#define HOLEMEDIUMMASS HOLESMALLMASS*3
#define HOLELARGEMASS HOLESMALLMASS*5
#define MAXSTARTRESERVE MAX_PUT_HOLES*HOLESMALLMASS

#define MAXTIME 1000 /* in Timesteps */

#define BULGESIZE 0.015f
#define STARSIZE 0.0028f
#define HOLESIZE 0.018f

typedef struct
{
  double x, y, z; /* in 600000 LJ (0.5 = 300000 LJ = Mitte vom Spielfeld) */
  double vx, vy, vz; /* in m/s */
  double fx, fy, fz; /* Kraft in Newton auf den Koerper */
  double mass; /* in Sonnenmassen */
  int inLevel; /* ob das Objekt durch das Level vorgegeben ist */
  int exists;
  int nograv;
} skymass;

typedef struct
{
  double x, y, z; /* in 600000 LJ (0.5 = 300000 LJ = Mitte vom Spielfeld) */
  double r; /* Radius */  
} skygoal;

typedef struct
{
  skygoal goal;
  skymass holes[MAX_LEVEL_HOLES+MAX_PUT_HOLES];
  skymass galaxies[MAX_GALAXIES];
  skymass *stars;
  int holesSize;
  int galaxiesSize;
  int starsSize;
  double massreserve;
} Universe;

#endif

