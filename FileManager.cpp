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
#include "FileManager.h"

std::string FileManager::getFile()
{
  name = "";
  doinput = true;
  /*std::cout << "Save as: ";
  std::cin >> name;*/
  int i = 0;
  while(doinput)
  {
    i++;
    draw(i);
    handleEvents();
    usleep(100000);
  }
  return name;
}

void FileManager::draw(int i)
{ 
  display.getDisplay()->cleanDisplay();
  display.drawBridge(universe, SpaceDisplay::EditorView, 100);
  display.getDisplay()->OrthoMode(); 

  std::string sname = "Save this Level as " + name;
  if((i/2) % 2)
  {
    sname = sname + "_";
  }
  display.getIllustrator()->glPrint(30.0, 0.0, 1.0, 1.0, 10.0, 120.0, sname.c_str());
  SDL_GL_SwapBuffers();
}

void FileManager::handleEvents()
{
  SDL_Event event;
  
  while ( SDL_PollEvent( &event ) )
  {
    display.getDisplay()->handleEvents(event);
    switch( event.type )
    {
#if 0
      case SDL_MOUSEBUTTONDOWN:
        /* Buttons */
        if(view == SpaceDisplay::PutView || view == SpaceDisplay::SimulationView || view == SpaceDisplay::EditorView)
        {
          buttons.checkButtons(flags,event.motion.x,event.motion.y);
        }
              
        /* Nur fuer Setzfenster */
        if(view == SpaceDisplay::PutView || view == SpaceDisplay::EditorView)
        {        
          /* Objekt setzen? */
          if(event.motion.x > UNIVERSE_LEFT && 
             event.motion.x < display.getWidth()-(UNIVERSE_RIGHT+UNIVERSE_LEFT) && 
             event.motion.y > UNIVERSE_TOP && 
             event.motion.y < display.getHeight()-(UNIVERSE_TOP+UNIVERSE_BOTTOM)
            )
          {                        
            /* Mausposition umrechnen */
            glGetIntegerv(GL_VIEWPORT,viewport);
            glGetDoublev(GL_PROJECTION_MATRIX,projMatrix);
            glGetDoublev(GL_MODELVIEW_MATRIX,modelMatrix);
            glReadPixels(event.motion.x, event.motion.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &zpos );
            gluUnProject(event.motion.x, event.motion.y, zpos,
                modelMatrix, projMatrix, viewport,
                &mousex, &mousey, &mousez
            );
            
            mousey = 1.0-mousey;

            editor.check(mousex,mousey);
          }
        }
        break;
#endif
      case SDL_KEYDOWN:
        if((event.key.keysym.sym >= 'a' && event.key.keysym.sym <= 'z') || (event.key.keysym.sym >= '0' && event.key.keysym.sym <= '9'))
        {
          name += toupper(event.key.keysym.sym);
        } 
	else if (event.key.keysym.sym == SDLK_RETURN)
        {
          doinput = false;
        }
        break;
      default:
        break;
    }
  }
}
