#ifndef _EDITOR_H_
#define _EDITOR_H_

#include "ButtonMaster.h"
#include "ButtonFlags.h"
#include "Universe.h"

class Editor
{
  private:
    Universe &uni;
    double maxreserve;
    bool all;
    double massreserve;

    enum {
      small,
      medium,
      large
    } size;

  public:
    Editor(Universe &universe, bool allowall = false);
    void setAllowAll(bool allowall){all=allowall;}
    void check(double mousex, double mousey);
    void parseButtons(ButtonFlags &flags);
    double getHoleWeight();
    double getQuotient();
};

#endif
