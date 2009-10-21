/* 
 * This file is part of Spacehero.
 * 
 * Spacehero is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Spacehero is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Spacehero.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _EDITOR_H_
#define _EDITOR_H_

#include "ButtonMaster.h"
#include "ButtonFlags.h"
#include "Universe.h"
#include "SpaceDisplay.h"

class Editor
{
  public:
    enum Size {
      small,
      medium,
      large
    };

    enum Type {
      hole,
      bulge,
      goal
    };
  private:
    Universe &uni;
    double maxreserve;
    bool all;
    double massreserve;

    bool setGalaxy;
    int galaxyX;
    int galaxyY;
    bool putting;

    Size size;
    Type type;
    double zoom;
  public:
    Editor(Universe &universe);
    void setAllowAll(bool allowall){all=allowall;}
    bool isAllowAll(){return all;}
    bool settingGalaxy(){return setGalaxy;}
    void check(double mousex, double mousey, int pixelx, int pixely, bool click, bool onSpace);
    void drawMouse(SpaceDisplay* display);
    void parseButtons(ButtonFlags &flags);
    double getHoleWeight();
    double getBulgeWeight();
    double getGoalRadius();
    double getSize();
    double getQuotient();
    Editor::Type getType(){return type;}
    SpaceDisplay::BridgeView getView();
};

#endif
