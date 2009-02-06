/** @file galaxy.h
 * @brief Berechnung der Simulation und der Galaxien.
 */

#ifndef GALAXY_H
#define GALAXY_H

#include "local.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define R_MIN_CENTER 0.02
#define R_MIN 0.0005
#define A 200
#define M 5

#define ORBITS_MAX 130

/** Lässt eingesogene Himmelsobjekte verschwinden.
  * @param[in,out] stars[] Die Sterne.
  * @param[in] starsSize Anzahl der Sterne.
  * @param[in,out] holes[] Die schwarzen Löcher.
  * @param[in] holesSize Anzahl der schwarzen Löcher.
  * @param[in,out] galaxies[] Die Galaxien.
  * @param[in] galaxiesSize Anzahl der Galaxien.
  */
void eventHorizon(skymass stars[], int starsSize, skymass holes[], int holesSize, skymass galaxies[], int galaxiesSize);

/** Konstruiert eine Galaxie.
  * @param[in,out] galaxy Der Galaxienmittelpunkt.
  * @param[in,out] **INstars Pointer auf Sterne.
  * @param[in] *starsSize Anzahl der Sterne.
  */
void constructGalaxy(skymass *galaxy, skymass **INstars, int *starsSize);

/** Lässt eingesogene Himmelsobjekte verschwinden.
  * @param[in,out] galaxies[] Die Galaxien.
  * @param[in] galaxiesSize Anzahl der Galaxien.
  * @param[in,out] holes[] Die schwarzen Löcher.
  * @param[in] holesSize Anzahl der schwarzen Löcher.
  * @param[in,out] stars[] Die Sterne.
  * @param[in] starsSize Anzahl der Sterne.
  * @param[in] time Die aktuelle Simulationszeit.
  */
void move(skymass galaxies[], int galaxiesSize, skymass holes[], int holesSize, skymass stars[], int starsSize, int time);

#endif

