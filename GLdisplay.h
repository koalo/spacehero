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
#ifndef _GLDISPLAY_H_
#define _GLDISPLAY_H_

#include <SDL.h>

class GLdisplay
{
  private:
    GLdisplay(const GLdisplay&);
    const GLdisplay& operator=(const GLdisplay&);

  private:
    SDL_Surface *surface;
    int videoFlags; /* Flags fuer Videoinitialisierung */
    const SDL_VideoInfo *videoInfo; /* Infos ueber Video */
   
  private:
    int width;
    int height;
    int bpp;

  public:
    GLdisplay(bool fullscreen=false, int width=600, int height=600, int bpp=16);
    virtual ~GLdisplay();

    void resizeWindow(int width, int height);
    int getWidth();
    int getHeight();
    void cleanDisplay();
    void OrthoMode();
    void PerspectiveMode(int left, int top, int width, int height, float angle);
    void handleEvents(SDL_Event &event);
    void waitForUser();
};

#endif
