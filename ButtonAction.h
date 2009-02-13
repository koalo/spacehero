#ifndef BUTTONACTION_H
#define BUTTONACTION_H

  typedef enum {
    b_smallHole = 1,
    b_mediumHole = 2,
    b_largeHole = 4,
    b_startSimulation = 8,
    b_breakSimulation = 16,
    b_replaySimulation = 32,
    b_breakIntro = 64,
    b_exit = 128
  } ButtonAction;

class ButtonAction : public AbstractButtonAction
{
}

#endif