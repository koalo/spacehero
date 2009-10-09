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

#include "ButtonMaster.h"

ButtonMaster::ButtonMaster(PictureBook &t, Illustrator &i)
  : buttonSize(0), textures(t), illustrator(i)
{
}

void ButtonMaster::clearButtons()
{
  buttonSize = 0;
}

void ButtonMaster::addButton(std::string texture, float x, float y, float r, ButtonFlags::Actions action)
{
  Button newButton;

  newButton.x = x;
  newButton.y = y;
  newButton.r = r;
  newButton.texture = texture;
  newButton.active = 1;
  newButton.action = action;
  buttons[buttonSize++] = newButton;
}

void ButtonMaster::drawButtons()
{
  int i;

  for(i = 0; i < buttonSize; i++)
  {
    /*    glEnable( GL_BLEND );
          glBlendFunc(GL_SRC_ALPHA,GL_ONE);
          drawDisk(IMG_BULGE,display->buttons[i].x,display->buttons[i].y,display->buttons[i].r*3,display);
          glDisable( GL_BLEND );*/
    textures.useTexture(buttons[i].texture);
    illustrator.drawDisk(buttons[i].x,buttons[i].y,buttons[i].r);
  }
}

void ButtonMaster::checkButtons(ButtonFlags &flags, int x, int y)
{
  int i;

  for(i = 0; i < buttonSize; i++)
  {
    if( hypot(buttons[i].x-x,buttons[i].y-y) < buttons[i].r)
    {
      flags.activateFlag((AbstractButtonFlags::Actions)buttons[i].action);
    }
  }
}

void AbstractButtonFlags::activateFlag(Actions action)
{
  flags = flags | action;
}

bool AbstractButtonFlags::checkFlag(int action)
{
  bool ret = ((flags & action) == action);
  flags = flags & ~action;
  return ret;
}
 

bool AbstractButtonFlags::viewFlag(int action)
{
  return ((flags & action) == action);
}

bool AbstractButtonFlags::isFlag()
{
  return (flags != 0);
}
