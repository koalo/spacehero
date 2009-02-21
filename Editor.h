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
