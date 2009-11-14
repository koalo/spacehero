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

#include <GL/gl.h>
#include <math.h>

AbstractButtonFlags::AbstractButtonFlags()
  : flags(0)
{
}

Button::Button()
  : texture(""), x(0), y(0), r(0), active(0), action((ButtonFlags::Actions)4096)
{
}

ButtonMaster::ButtonMaster(PictureBook &t, Illustrator &i)
  : buttons(), textures(t), illustrator(i), pressedButton("") 
{
}

void ButtonMaster::clearButtons()
{
  buttons.clear();
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
  buttons.push_back(newButton);
}

void ButtonMaster::drawButtons()
{
  for(unsigned int i = 0; i < buttons.size(); i++)
  {
    if(textures.isTexture(buttons.at(i).texture))
    {
      textures.useTexture(buttons.at(i).texture);
      illustrator.drawDisk(buttons.at(i).x,buttons.at(i).y,buttons.at(i).r);
    } else {
      //textures.useTexture("button",buttons.at(i).texture == pressedButton);
      textures.useTexture("button");
      glEnable( GL_BLEND );
      glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
      illustrator.putImage(buttons.at(i).x-buttons.at(i).r,buttons.at(i).y-buttons.at(i).r,buttons.at(i).r*2,buttons.at(i).r*2);
      glDisable( GL_BLEND );
      illustrator.setFontheight(buttons.at(i).r*0.4);
      float length = buttons.at(i).texture.length()*illustrator.getFontwidth();
      glColor3f(0.0,0.0,0.0);
      illustrator.glPrint(buttons.at(i).x-length*0.5-illustrator.getFontwidth()*0.35, buttons.at(i).y-illustrator.getFontwidth(), buttons.at(i).texture.c_str());
      glColor3f(1.0,1.0,1.0);
    }
  }
}

void ButtonMaster::checkButtons(ButtonFlags &flags, int x, int y)
{
  for(unsigned int i = 0; i < buttons.size(); i++)
  {
    if( hypot(buttons.at(i).x-x,buttons.at(i).y-y) < buttons.at(i).r)
    {
      flags.activateFlag((AbstractButtonFlags::Actions)buttons.at(i).action);
      pressedButton = buttons.at(i).texture;
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
