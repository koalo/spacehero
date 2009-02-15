#ifndef BUTTONFLAGS_H
#define BUTTONFLAGS_H

class ButtonFlags : public AbstractButtonFlags
{
  public:
  enum Actions {
    smallHole = 1,
    mediumHole = 2,
    largeHole = 4,
    startSimulation = 8,
    breakSimulation = 16,
    replaySimulation = 32,
    breakIntro = 64,
    exit = 128
  };
};

#endif
