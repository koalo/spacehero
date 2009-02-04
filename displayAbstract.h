#ifndef DISPLAYABSTRACT_H
#define DISPLAYABSTRACT_H

#include "local.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include "SDL.h"
#include <math.h>

#include "buttons.h"

/* screen width, height, and bit depth */
#define SCREEN_WIDTH  600
#define SCREEN_HEIGHT 600
#define SCREEN_BPP     16

#define IMG_STAR 0
#define IMG_HOLE 1
#define IMG_BACKGROUND 2
#define IMG_BLACK 3
#define IMG_GOAL 4
#define IMG_PANEL_MASS 5
#define IMG_PANEL_TIME 6
#define IMG_BUTTON 7
#define IMG_START 8
#define IMG_STOP 9
#define IMG_REPLAY 10
#define IMG_EXIT 11
#define IMG_SPACEHERO 12
#define IMG_BULGE 13
#define IMG_WIN 14
#define IMG_LOST 15
#define IMG_ACTIVE 16

#define TEXTURES 17

#define MAX_BUTTONS 10

typedef struct
{
  int texture;
  float x;
  float y;
  float r;
  int active;
  void (*action)(BStatus *state);
} Button;

typedef struct
{
  SDL_Surface *surface;
  GLuint texture[TEXTURES]; /* Texturspeicher */  
  int videoFlags; /* Flags fuer Videoinitialisierung */
  SDL_Event event;
  const SDL_VideoInfo *videoInfo; /* Infos ueber Video */
  int isActive; /* Fenster hat Fokus? */
  float zoom;
  int height;
  int width;
  Button buttons[MAX_BUTTONS];
  int buttonSize;
  BStatus state;
  GLuint fontbase;
} GLdisplay;

typedef struct
{
  float rx;
  float ry;
  float rz;
} Kamera;

#include "glprint.h"

void initDisplay(GLdisplay *display);
void resizeWindow(GLdisplay *display, int width, int height);
void putImage(int texture, float x, float y, float width, float height, GLdisplay *display);
void drawRect(float red, float green, float blue, float x, float y, float width, float height);
void drawSphere(int texture, float x, float y, float r, GLdisplay *display);
void drawDisk(int texture, float x, float y, float r, GLdisplay *display);
void exitApp(GLdisplay *display);

void addButton(GLdisplay *display, int texture, float x, float y, float r, void (*action)(BStatus *state));
void clearButtons(GLdisplay *display);
void drawButtons(GLdisplay *display);
void checkButtons(GLdisplay *display);

#endif

