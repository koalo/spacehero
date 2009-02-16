#ifndef _EDITOR_H_
#define _EDITOR_H_

#include "Universe.h"

class Editor
{
  private:
    Universe &uni;
    double maxreserve;

  public:
    Editor(Universe &universe);
    double massreserve;
    double holeWeight;
    void check(double mousex, double mousey);
    double getQuotient();
};

#endif
