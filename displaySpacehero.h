/** @file displaySpacehero.h
 * @brief OpenGL-Darstellungsfunktionen für das Spiel.
 */

#ifndef DISPLAYSPACEHERO_H
#define DISPLAYSPACEHERO_H

#include "local.h"
#include "displayAbstract.h"

#define PUT 3463
#define SIMULATION 46584

#define UNIVERSE_LEFT 0
#define UNIVERSE_TOP 0
#define UNIVERSE_BOTTOM 0

#define PANELRATIO 0.205962059620
#define UNIVERSE_RIGHT (PANELRATIO*display->height)

#define START_BUTTON (UNIVERSE_RIGHT*0.4)
#define REPLAY_BUTTON (UNIVERSE_RIGHT*0.2)
#define EXIT_BUTTON (UNIVERSE_RIGHT*0.1)
#define MARGIN 10

#define SMALL_HOLE (UNIVERSE_RIGHT*0.1)
#define MEDIUM_HOLE (UNIVERSE_RIGHT*0.12)
#define LARGE_HOLE (UNIVERSE_RIGHT*0.14)

#define VIEWANGLE 45

#define ORTHOGONAL 56345
#define PERSPECTIVE 34523

#define TEXTSPACE 15
#define TEXTR 0.0f
#define TEXTG 1.0f
#define TEXTB 0.0f

/** Zeichnet das Universum.
  * @param[in,out] *display Pointer aufs Display.
  * @param[in] uni Das Universum.
  * @param[in] projection Projektion (entweder PERSPECTIVE oder ORTHOGONAL).
  * @param[in] width Breite des Zeichenbereichs.
  * @param[in] height Höhe des Zeichenbereichs.
  */
void displayUniverse( GLdisplay *display, Universe *uni, int projection, int width, int height );

/** Zeichnet die Oberfläche zum schwarze Löcher setzen.
  * @param[in,out] *display Pointer aufs Display.
  * @param[in] uni Das Universum.
  */
void drawPut(GLdisplay *display, Universe *uni);

/** Zeichnet die Oberfläche der Simulation.
  * @param[in,out] *display Pointer aufs Display.
  * @param[in] uni Das Universum.
  * @param[in] cam Die Kamera (wird noch nicht verwendet!).
  * @param[in] time Die aktuelle Simulationszeit.
  */
void drawSimulation( GLdisplay *display, Universe *uni, Kamera *cam, int time );

/** Überprüft auf Events.
  * @param[in,out] *display Pointer aufs Display.
  * @param[in] part Gibt an in welchem Teil dies 
  * @param[in] uni Das Universum.
  */
void handleEvents(GLdisplay *display, int part, Universe *uni);

#endif

