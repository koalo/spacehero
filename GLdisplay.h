#ifndef _GLDISPLAY_H_
#define _GLDISPLAY_H_

#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
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
};

#endif
