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
#ifndef _BUTTONMASTER_H_
#define _BUTTONMASTER_H_

#include <string>
#include <vector>
using namespace std;

#include "PictureBook.h"
#include "Illustrator.h"

class AbstractButtonFlags
{
  public:
    /* muss von Kindklassen gefuellt werden */
    enum Actions {
    };

  private:
    int flags;
  
  public:
    AbstractButtonFlags();
    virtual ~AbstractButtonFlags() {};

    void activateFlag(Actions action);
    
    /* statt int waer enum besser, aber geht nicht da kein Enum-Prototyping */
    bool checkFlag(int action); /* loescht auch das Flag */  
    bool viewFlag(int action); /* loescht nicht */

    bool isFlag();
};

#include "ButtonFlags.h"

class Button
{
  public:
    string texture;
    float x;
    float y;
    float r;
    int active;
    ButtonFlags::Actions action;
    Button();
};

class ButtonMaster
{
  private:
    vector<Button> buttons;    
    PictureBook &textures;
    Illustrator &illustrator;

  public:
    void addButton(string texture, float x, float y, float r, ButtonFlags::Actions action);
    void clearButtons();
    void drawButtons();
    void checkButtons(ButtonFlags &flags, int x, int y);

    ButtonMaster(PictureBook &t, Illustrator &i);
};

#endif
