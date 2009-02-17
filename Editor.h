#ifndef _EDITOR_H_
#define _EDITOR_H_

#include "ButtonMaster.h"
#include "ButtonFlags.h"
#include "Universe.h"
#include "SpaceDisplay.h"

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

    enum {
      hole,
      bulge,
      goal
    } type;

  public:
    Editor(Universe &universe);
    void setAllowAll(bool allowall){all=allowall;}
    bool isAllowAll(){return all;}
    void check(double mousex, double mousey);
    void parseButtons(ButtonFlags &flags);
    double getHoleWeight();
    double getBulgeWeight();
    double getGoalRadius();
    double getQuotient();
    SpaceDisplay::BridgeView getView();
};

#endif
