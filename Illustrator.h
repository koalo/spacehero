#ifndef _ILLUSTRATOR_H_
#define _ILLUSTRATOR_H_

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL_image.h>
#include <SDL.h>
#include <iostream>

class Illustrator
{
  private:
    GLuint fontbase;
    GLuint font;

  public:
    Illustrator();
    ~Illustrator();

    void putImage(float x, float y, float width, float height);
    void drawRect(float red, float green, float blue, float x, float y, float width, float height);
    void drawSphere(float x, float y, float r);
    void drawDisk(float x, float y, float r);

    void buildFont();
    void glPrint(float red, float green, float blue, float x, float y, const char *format, ... );
};

#endif
