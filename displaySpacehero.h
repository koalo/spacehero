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

void displayUniverse( GLdisplay *display, Universe *uni, int projection, int width, int height );
void drawPut(GLdisplay *display, Universe *uni);
void drawSimulation( GLdisplay *display, Universe *uni, Kamera *cam, int time );
void handleEvents(GLdisplay *display, int part, Universe *uni);

#endif

