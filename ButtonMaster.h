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

#include <GL/gl.h>
#include <SDL.h>
#include <iostream>
#include <math.h>

#include "PictureBook.h"
#include "Illustrator.h"
#include "GLdisplay.h"

class AbstractButtonFlags
{
  public:
  /* muss von Kindklassen gefuellt werden */
  enum Actions {
  };

  private:
  int flags;
  
  public:
  void activateFlag(Actions action);
  bool checkFlag(int action); /* statt int waer enum besser, aber geht nicht da kein Enum-Prototyping - loescht auch das Flag */
  bool viewFlag(int action); /* loescht nicht! */
  AbstractButtonFlags()
    : flags(0)
  {
  }
  virtual ~AbstractButtonFlags() {};
};

#include "ButtonFlags.h"

class Button
{
  public:
   std::string texture;
   float x;
   float y;
   float r;
   int active;
   ButtonFlags::Actions action;
   Button() : texture(""), x(0), y(0), r(0), active(0), action((ButtonFlags::Actions)4096) {}
};

class ButtonMaster
{
  private:
    #define MAX_BUTTONS 10
    Button buttons[MAX_BUTTONS];
    int buttonSize;
    PictureBook &textures;
    Illustrator &illustrator;

   public:
    void addButton(std::string texture, float x, float y, float r, ButtonFlags::Actions action);
    void clearButtons();
    void drawButtons();
    void checkButtons(ButtonFlags &flags, int x, int y);

    ButtonMaster(PictureBook &t, Illustrator &i);
};

#endif
