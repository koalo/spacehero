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

#include "GLdisplay.h"
#include "PictureBook.h"
#include "Illustrator.h"
#include "ButtonMaster.h"
#include "Universe.h"

class Editor;

class SpaceDisplay
{
  public:
    SpaceDisplay(string path);

  public:
    enum BridgeView { 
      SimulationView,
      PutView,
      EditorView,
      IntroView,
      ScreenView,
      MenuView
    };

    void displayUniverse( Universe &uni, int width, int height, bool eye = false, bool pleft = true, bool arrows = false );
    void initBridge(Universe &uni, BridgeView view);
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
    inline void drawSkymass(SkyMass body);
    
  public:
    Illustrator* getIllustrator(){return &illustrator;}
    GLdisplay* getDisplay(){return &display;}
    PictureBook* getPictureBook(){return &textures;}
};

#include "Editor.h"

#endif
