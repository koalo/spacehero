/** @file glprint.h
 * @brief Ausgabe von Text.
 */

#ifndef GLPRINT_H
#define GLPRINT_H
#include <string.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include "SDL.h"

#include "displayAbstract.h"

/** Räumt den Fontspeicher auf.
  * @param[in,out] *display Das Display.
  */
GLvoid KillFont( GLdisplay *display );

/** Läd die Font ein.
  * @param[in,out] *display Das Display.
  */
GLvoid buildFont( GLdisplay *display );

/** Gibt Text auf dem Bildschirm aus.
  * @param[in,out] *display Das Display.
  * @param[in] red Rotanteil.
  * @param[in] green Grünanteil.
  * @param[in] blue Blauanteil.
  * @param[in] x X-Position.
  * @param[in] y Y-Position.
  * @param[in] *format Formatstring.
  * @param[in] ... Parameter.
  */
GLvoid glPrint( GLdisplay *display, float red, float green, float blue, float x, float y, const char *format, ... );
 
#endif
