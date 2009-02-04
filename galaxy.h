#ifndef GALAXY_H
#define GALAXY_H

#include "local.h"

void eventHorizon(skymass stars[], int starsSize, skymass holes[], int holesSize, skymass galaxies[], int galaxiesSize);
void constructGalaxy(skymass *galaxy, skymass **INstars, int *starsSize);
void move(skymass galaxies[], int galaxiesSize, skymass holes[], int holesSize, skymass stars[], int starsSize, int time);

#endif

