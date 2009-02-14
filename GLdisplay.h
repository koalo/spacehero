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
#include <math.h>

#include "Universe.h"

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
 	  int texture;
 	  float x;
	  float y;
	  float r;
	  int active;
	  ButtonFlags::Actions action;
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
  public:
    enum Images {
   		IMG_STAR = 0,
    	IMG_HOLE,
    	IMG_BLACK,
    	IMG_GOAL,
    	IMG_PANEL_MASS,
    	IMG_PANEL_TIME,
    	IMG_BUTTON,
    	IMG_START,
    	IMG_STOP,
    	IMG_REPLAY,
    	IMG_EXIT,
    	IMG_SPACEHERO,
    	IMG_BULGE,
    	IMG_WIN,
    	IMG_LOST,
    	IMG_ACTIVE,
    	TEXTURES
    };

  private:
    SDL_Surface *surface;
    int videoFlags; /* Flags fuer Videoinitialisierung */
   const SDL_VideoInfo *videoInfo; /* Infos ueber Video */
   
  protected:
    int isActive; /* Fenster hat Fokus? */
    SDL_Event event;
    GLuint texture[TEXTURES]; /* Texturspeicher */  
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

    void putImage(Images t, float x, float y, float width, float height);
    void drawRect(float red, float green, float blue, float x, float y, float width, float height);
    void drawSphere(Images t, float x, float y, float r);
    void drawDisk(int t, float x, float y, float r);

    void addButton(Images t, float x, float y, float r, ButtonFlags::Actions action);
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
