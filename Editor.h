#ifndef _EDITOR_H_
#define _EDITOR_H_

#include "Universe.h"

class Editor
{
  public:
    double massreserve;
    double holeWeight;
    void check(double mousex, double mousey, Universe &universe);
    Editor():massreserve(MAXSTARTRESERVE),holeWeight(HOLEMEDIUMMASS){};
};

#endif
