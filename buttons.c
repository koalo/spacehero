#include "buttons.h"

void clearStatus(BStatus *state)
{
  state->startSimulation = 0;
  state->breakSimulation = 0;
  state->breakIntro = 0;
  state->exit = 0;
}

void BUT_smallHole(BStatus *state)
{
  state->holeWeight = HOLESMALLMASS;
}

void BUT_mediumHole(BStatus *state)
{
  state->holeWeight = HOLEMEDIUMMASS;
}

void BUT_largeHole(BStatus *state)
{
  state->holeWeight = HOLELARGEMASS;
}

void BUT_startSimulation(BStatus *state)
{
  state->startSimulation = 1;
}

void BUT_breakSimulation(BStatus *state)
{
  state->breakSimulation = 1;
}

void BUT_replaySimulation(BStatus *state)
{
  state->replaySimulation = 1;
}

void BUT_breakIntro(BStatus *state)
{
  state->breakIntro = 1;
}

void BUT_exit(BStatus *state)
{
  state->exit = 1;
}

