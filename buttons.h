/** @file buttons.h
 * @brief Button-Handling.
 */

#ifndef BUTTONS_H
#define BUTTONS_H

#include "local.h"

typedef struct
{
  int startSimulation;
  float holeWeight;
  int breakSimulation;
  int replaySimulation;
  int breakIntro;
  int exit;
} BStatus;

/** Setzt die Statusflags der Buttons zurueck.
  * @param[in,out] *state Der Statuspointer.
  */
void clearStatus(BStatus *state);

/** Setzt Masse von zu setztenden Galaxien klein.
  * @param[in,out] *state Der Statuspointer.
  */
void BUT_smallHole(BStatus *state);

/** Setzt Masse von zu setztenden Galaxien mittel.
  * @param[in,out] *state Der Statuspointer.
  */
void BUT_mediumHole(BStatus *state);

/** Setzt Masse von zu setztenden Galaxien gross.
  * @param[in,out] *state Der Statuspointer.
  */
void BUT_largeHole(BStatus *state);

/** Setzt das Simulation starten Flag.
  * @param[in,out] *state Der Statuspointer.
  */
void BUT_startSimulation(BStatus *state);

/** Setzt das Simulation abbrechen Flag.
  * @param[in,out] *state Der Statuspointer.
  */
void BUT_breakSimulation(BStatus *state);

/** Setzt das Simulation neustarten Flag.
  * @param[in,out] *state Der Statuspointer.
  */
void BUT_replaySimulation(BStatus *state);

/** Setzt das Intro abbrechen Flag.
  * @param[in,out] *state Der Statuspointer.
  */
void BUT_breakIntro(BStatus *state);

/** Setzt das Exit Flag.
  * @param[in,out] *state Der Statuspointer.
  */
void BUT_exit(BStatus *state);
#endif
