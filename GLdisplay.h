#ifndef _GLDISPLAY_H_
#define _GLDISPLAY_H_

class AbstractButtonFlags;
class Button;
class GLdisplay;
class Kamera;

#include <iostream>
#include <fstream>

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>
#include <SDL_image.h>
#include <math.h>

#include "Universe.h"
#include "PictureBook.h"

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


class Kamera
{
	public:
    float rx;
    float ry;
    float rz;
};

class GLdisplay
{
  private:
    GLdisplay(const GLdisplay&);
    const GLdisplay& operator=(const GLdisplay&);

  protected:
    PictureBook textures;

  private:
    SDL_Surface *surface;
    int videoFlags; /* Flags fuer Videoinitialisierung */
    const SDL_VideoInfo *videoInfo; /* Infos ueber Video */
   
  protected:
    int isActive; /* Fenster hat Fokus? */
    SDL_Event event;
    float zoom;
    int width;
    int height;
    int bpp;

  private:
    #define MAX_BUTTONS 10
    Button buttons[MAX_BUTTONS];
    int buttonSize;
    GLuint fontbase;

  public:
    GLdisplay(std::string path, bool fullscreen=false, int width=600, int height=600, int bpp=16);
    virtual ~GLdisplay();

    void resizeWindow(int width, int height);

    void putImage(float x, float y, float width, float height);
    void drawRect(float red, float green, float blue, float x, float y, float width, float height);
    void drawSphere(float x, float y, float r);
    void drawDisk(float x, float y, float r);

    void addButton(std::string texture, float x, float y, float r, ButtonFlags::Actions action);
    void clearButtons();
    void drawButtons();
    void checkButtons(ButtonFlags &flags);

    void KillFont(); /* TODO in Destruktor*/
    void buildFont();
    void glPrint(float red, float green, float blue, float x, float y, const char *format, ... );

  private:
    int LoadGLTextures(GLuint texture[],std::string path);
};



#endif
