/* 
 * This file is part of Spacehero.
 * 
 * Spacehero is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Spacehero is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Spacehero.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _SPACEDISPLAY_H_
#define _SPACEDISPLAY_H_

#define UNIVERSE_LEFT 0
#define UNIVERSE_TOP 0
#define UNIVERSE_BOTTOM 0

#define PANELRATIO 0.205962059620
#define UNIVERSE_RIGHT (PANELRATIO*display.getHeight())

#define START_BUTTON (UNIVERSE_RIGHT*0.4)
#define REPLAY_BUTTON (UNIVERSE_RIGHT*0.2)
#define EXIT_BUTTON (UNIVERSE_RIGHT*0.1)
#define MARGIN 10

#define SMALL_HOLE (UNIVERSE_RIGHT*0.1)
#define MEDIUM_HOLE (UNIVERSE_RIGHT*0.12)
#define LARGE_HOLE (UNIVERSE_RIGHT*0.14)

#define VIEWANGLE 60

#define TEXTR 0.0f
#define TEXTG 1.0f
#define TEXTB 0.0f

#include "GLdisplay.h"
#include "Universe.h"
#include "ButtonMaster.h"
#include "PictureBook.h"
#include "Illustrator.h"


class Editor;

class SpaceDisplay
{
  public:
    SpaceDisplay(std::string path);

  public:
    enum BridgeView { 
      SimulationView,
      PutView,
      EditorView,
      IntroView,
      MenuView
    };

    void displayUniverse( Universe &uni, int width, int height, bool eye = false, bool pleft = true, bool arrows = false );
    void drawBridge(Universe &uni, BridgeView view, double indicator, double holeWeight = 0);
    void handleEvents(BridgeView view, ButtonFlags &flags, Editor &editor);
    void showEnd(bool win, ButtonFlags &flags);
    void showMenu(double time);

  private:
    GLdisplay display;
    PictureBook textures;
    Illustrator illustrator;
    ButtonMaster buttons;

    void alignSimulButtons();
    void alignPutButtons();
    int isActive; /* Fenster hat Fokus? */
    SDL_Event event;
    float zoom;

    inline void drawStars(bool behind, bool eye, bool pleft, Universe &uni);

    inline void drawSkymass(SkyMass body)
    {
      double z = body.z;
      z = 8000*z*z;
      if(body.z < 0) z = -z;
      glBegin(GL_QUADS);
      glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -body.radius+body.x, -body.radius+body.y, z );
      glTexCoord2f( 1.0f, 0.0f ); glVertex3f(  body.radius+body.x, -body.radius+body.y, z );
      glTexCoord2f( 1.0f, 1.0f ); glVertex3f(  body.radius+body.x,  body.radius+body.y, z );
      glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -body.radius+body.x,  body.radius+body.y, z );
      glEnd();
    };
    
  public:
    Illustrator* getIllustrator(){return &illustrator;}
    GLdisplay* getDisplay(){return &display;}
    PictureBook* getPictureBook(){return &textures;}
};

#include "Editor.h"

#endif
