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
#ifndef _ILLUSTRATOR_H_
#define _ILLUSTRATOR_H_

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL_image.h>
#include <SDL.h>
#include <iostream>
#include <string>
#include <math.h>

class Illustrator
{
  private:
    GLuint fontbase;
    GLuint font;
    float fontsize;

  public:
    Illustrator(std::string path);
    ~Illustrator();

    void putImage(float x, float y, float width, float height);
    void drawLine(float sx, float sy, float ex, float ey, float width, bool arrow = false, float hypo = 30.0);
    void drawRect(float red, float green, float blue, float x, float y, float width, float height);
    void drawSphere(float x, float y, float r);
    void drawDisk(float x, float y, float r);

    void buildFont();
    void glPrint(float size, float red, float green, float blue, float x, float y, const char *format, ... );
    float getFontsize(){return fontsize;}
};

#endif
