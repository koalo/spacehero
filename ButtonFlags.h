#ifndef BUTTONFLAGS_H
#define BUTTONFLAGS_H

class ButtonFlags : public AbstractButtonFlags
{
  public:
  enum Actions {
    small = 1,
    medium = 2,
    large = 4,
    startSimulation = 8,
    breakSimulation = 16,
    replaySimulation = 32,
    breakIntro = 64,
    exit = 128,
    putHole = 256,
    putBulge = 512,
    putGoal = 1024,
    startEditor = 2048
  };
};

#endif
