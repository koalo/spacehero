#ifndef _SPACEDISPLAY_H_
#define _SPACEDISPLAY_H_

#define PUT 3463
#define SIMULATION 46584

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
    SpaceDisplay(std::string path);
  public:
    enum BridgeView { 
      SimulationView,
      PutView,
      EditorView
    };

    void displayUniverse( Universe &uni, int projection, int width, int height );
    void drawBridge(Universe &uni, BridgeView view);
    void handleEvents(BridgeView view, Universe &uni, ButtonFlags &flags);

  private:
    void alignSimulButtons();
    void alignPutButtons();

    inline void drawSkymass(SkyMass body, float size)
    {
      glBegin(GL_QUADS);
      glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -size+body.x, -size+body.y, body.z );
      glTexCoord2f( 1.0f, 0.0f ); glVertex3f(  size+body.x, -size+body.y, body.z );
      glTexCoord2f( 1.0f, 1.0f ); glVertex3f(  size+body.x,  size+body.y, body.z );
      glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -size+body.x,  size+body.y, body.z );
      glEnd();
      /* Objekt zeichnen */
      /*
      glBegin(GL_QUADS);
      glTexCoord2f( 0.0f, 0.0f ); glVertex3f( body.x, -size+body.y, -size+body.z );
      glTexCoord2f( 1.0f, 0.0f ); glVertex3f( body.x, -size+body.y, size+body.z );
      glTexCoord2f( 1.0f, 1.0f ); glVertex3f( body.x,  size+body.y, size+body.z );
      glTexCoord2f( 0.0f, 1.0f ); glVertex3f( body.x,  size+body.y, -size+body.z );
      glEnd();

      glBegin(GL_QUADS);
      glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -size+body.x, body.y, -size+body.z );
      glTexCoord2f( 1.0f, 0.0f ); glVertex3f( -size+body.x, body.y, size+body.z );
      glTexCoord2f( 1.0f, 1.0f ); glVertex3f( size+body.x,  body.y, size+body.z );
      glTexCoord2f( 0.0f, 1.0f ); glVertex3f( size+body.x,  body.y, -size+body.z );
      glEnd();*/
    };
};


#endif
