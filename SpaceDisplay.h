#ifndef _SPACEDISPLAY_H_
#define _SPACEDISPLAY_H_

enum {
PUT = 3463,
SIMULATION = 46584,
};

#define UNIVERSE_LEFT 0
#define UNIVERSE_TOP 0
#define UNIVERSE_BOTTOM 0

#define PANELRATIO 0.205962059620
#define UNIVERSE_RIGHT (PANELRATIO*this->height)

#define START_BUTTON (UNIVERSE_RIGHT*0.4)
#define REPLAY_BUTTON (UNIVERSE_RIGHT*0.2)
#define EXIT_BUTTON (UNIVERSE_RIGHT*0.1)
#define MARGIN 10

#define SMALL_HOLE (UNIVERSE_RIGHT*0.1)
#define MEDIUM_HOLE (UNIVERSE_RIGHT*0.12)
#define LARGE_HOLE (UNIVERSE_RIGHT*0.14)

#define VIEWANGLE 45

#define TEXTSPACE 15
#define TEXTR 0.0f
#define TEXTG 1.0f
#define TEXTB 0.0f

#include "GLdisplay.h"

class SpaceDisplay : public GLdisplay
{
  public:
    void displayUniverse( Universe &uni, int projection, int width, int height );
    void drawBridge(Universe &uni, int projection, int time);
    void handleEvents(int part, Universe &uni);
    void drawPut(Universe &uni);
    void drawSimulation( Universe &uni, Kamera *cam, int time );
  private:
    void alignSimulButtons();
    void alignPutButtons();
  public:
    void drawEditor(Universe &/*u*/) {};
    void drawSimulation(Universe &/*u*/) {};
};


#endif
