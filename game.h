/** @file game.h
 * @brief Steuerung einer Spielrunde.
 */

#ifndef GAME_H
#define GAME_H

#include "displayAbstract.h"
#include "displaySpacehero.h"
#include <unistd.h>
#include <time.h>
#include "galaxy.h"
#include "levelladen.h"

/** Lässt eingesogene Himmelsobjekte verschwinden.
  * @param[in,out] *display Das Display.
  * @param[in] *level Name des Levels.
  */
void startRound(GLdisplay *display, char *level);

#endif
