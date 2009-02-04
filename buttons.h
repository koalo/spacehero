#ifndef BUTTONS_H
#define BUTTONS_H

typedef struct
{
  int startSimulation;
  float holeWeight;
  int breakSimulation;
  int replaySimulation;
  int breakIntro;
  int exit;
} BStatus;

void clearStatus(BStatus *state);

void BUT_smallHole(BStatus *state);
void BUT_mediumHole(BStatus *state);
void BUT_largeHole(BStatus *state);
void BUT_startSimulation(BStatus *state);
void BUT_breakSimulation(BStatus *state);
void BUT_replaySimulation(BStatus *state);
void BUT_breakIntro(BStatus *state);
void BUT_exit(BStatus *state);
#endif
