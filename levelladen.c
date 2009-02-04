#include "levelladen.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "local.h"


int levelladen(char *indateiname, skymass galaxies[], int *galaxiesSize, skymass holes[], int *holesSize, skygoal *goal)
{
  FILE *stream;
  char dateiname[100];
  char typ;
  double x,y,z,vx,vy,vz,mass,r;
  int i=0, k=0;

  skymass body;
  body.inLevel = 1;
  body.exists = 1;

/*  printf("\n\t  L E V E L - P O O L");
  printf("\n\t (1)Level 1\n\n");
  printf("__________________________________\n");

  printf("\nWelches Level möchten sie laden?\n");
  printf("(Bsp.: level1.txt)\n");
  scanf("%s", &dateiname[6]);*/
  sprintf(dateiname, "level/%s.txt", indateiname);
  printf("Loading...   %s ...\n\n", dateiname);


  if((stream = fopen(dateiname, "r")) == NULL)
  {
    printf("Datei %s ist nicht vorhanden\n", dateiname);
    return 0;
  }

  if(!fscanf(stream, "%lf\n%lf\n%lf\n%lf\n", &x, &y, &z, &r))
  {
    printf("Falsches Levelformat!");
    return 0;
  }

  goal->x = x;
  goal->y = y;
  goal->z = z;
  goal->r = r;

  while(fscanf(stream, "%c\n%lf\n%lf\n%lf\n%lf\n%lf\n%lf\n%lf\n", &typ, &x, &y, &z, &vx, &vy, &vz, &mass)>=0)
   {
   
     body.x = x;
     body.y = y;
     body.z = z;
     body.vx = vx;
     body.vy = vy;
     body.vz = vz;
     body.mass = mass;
  
     if(typ == 'G')
     {
     galaxies[i++] = body;
     }
     else if(typ == 'H')
     {
     holes[k++] = body;
     }
 /* else if(typ == 'Z')
   {
   goal[i] = body;
   }*/ 
   
   }

  *galaxiesSize = i;
  *holesSize = k;

  return 1;
}

