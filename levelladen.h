/** @file levelladen.h
 * @brief Läd ein Level ein.
 */

#ifndef LEVELLADEN_H
#define LEVELLADEN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "local.h"

/** Läd ein Level ein.
  * @param[in] *indateiname Levelbezeichnung.
  * @param[in,out] galaxies[] Die Galaxien.
  * @param[in] galaxiesSize Anzahl der Galaxien.
  * @param[in,out] holes[] Die schwarzen Löcher.
  * @param[in] holesSize Anzahl der schwarzen Löcher.
  * @param[in,out] *goal Der Zielbereich.
  */
int levelladen(char *indateiname, skymass galaxies[], int *galaxiesSize, skymass holes[], int *holesSize, skygoal *goal);

#endif
