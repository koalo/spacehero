/** @file displayAbstract.h
 * @brief Abstrahiert die Darstellung mit SDL und OpenGL.
 */

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
#define IMG_BLACK 2
#define IMG_GOAL 3
#define IMG_PANEL_MASS 4
#define IMG_PANEL_TIME 5
#define IMG_BUTTON 6
#define IMG_START 7
#define IMG_STOP 8
#define IMG_REPLAY 9
#define IMG_EXIT 10
#define IMG_SPACEHERO 11
#define IMG_BULGE 12
#define IMG_WIN 13
#define IMG_LOST 14
#define IMG_ACTIVE 15

#define TEXTURES 16

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

/** Initialisiert ein SDL-Display.
  * @param[in,out] *display Pointer aufs Display.
  */
void initDisplay(GLdisplay *display);

/** Initialisiert ein neues Display beim Ändern der Fenstergröße.
  * @param[in,out] *display Pointer aufs Display.
  * @param[in] width Breite des neuen Fensters.
  * @param[in] height Höhe des neuen Fensters.
  */
void resizeWindow(GLdisplay *display, int width, int height);

/** Zeichnet ein texturiertes Rechteck.
  * @param[in] texture Texturnummer.
  * @param[in] x X-Koordinate.
  * @param[in] y Y-Koordinate.
  * @param[in] width Breite.
  * @param[in] height Höhe.
  * @param[in,out] *display Pointer aufs Display.
  */
void putImage(int texture, float x, float y, float width, float height, GLdisplay *display);

/** Zeichnet ein farbiges Rechteck.
  * @param[in] red Rotanteil.
  * @param[in] green Grünanteil.
  * @param[in] blue Blauanteil.
  * @param[in] x X-Koordinate.
  * @param[in] y Y-Koordinate.
  * @param[in] width Breite.
  * @param[in] height Höhe.
  */
void drawRect(float red, float green, float blue, float x, float y, float width, float height);

/** Zeichnet eine texturierte Kugel.
  * @param[in] texture Texturnummer.
  * @param[in] x X-Koordinate.
  * @param[in] y Y-Koordinate.
  * @param[in] r Radius.
  * @param[in,out] *display Pointer aufs Display.
  */
void drawSphere(int texture, float x, float y, float r, GLdisplay *display);


/** Zeichnet einen texturierten Kreis.
  * @param[in] texture Texturnummer.
  * @param[in] x X-Koordinate.
  * @param[in] y Y-Koordinate.
  * @param[in] r Radius.
  * @param[in,out] *display Pointer aufs Display.
  */
void drawDisk(int texture, float x, float y, float r, GLdisplay *display);


/** Gibt Displayspeicher frei und beendet das Programm.
  * @param[in,out] *display Pointer aufs Display.
  */
void exitApp(GLdisplay *display);

/** Fügt einen Button ein.
  * @param[in,out] *display Pointer aufs Display.
  * @param[in] texture Texturnummer.
  * @param[in] x X-Koordinate.
  * @param[in] y Y-Koordinate.
  * @param[in] r Radius.
  * @param[in] (*action)(BStatus *state) Beim Klicken auszuführende Funktion.
  * @see buttons.h
  */
void addButton(GLdisplay *display, int texture, float x, float y, float r, void (*action)(BStatus *state));

/** Löscht alle Buttons.
  * @param[in,out] *display Pointer aufs Display.
  * @see buttons.h
  */
void clearButtons(GLdisplay *display);

/** Zeichnet alle Buttons.
  * @param[in,out] *display Pointer aufs Display.
  * @see buttons.h
  */
void drawButtons(GLdisplay *display);

/** Prüft ob Buttons angeklickt wurden und führt ggf. die Funktion aus.
  * @param[in,out] *display Pointer aufs Display.
  * @see buttons.h
  */
void checkButtons(GLdisplay *display);

#endif

