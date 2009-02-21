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
  while(doinput)
  {
    draw();
    handleEvents();
    usleep(100000);
  }
  return name;
}

void FileManager::draw()
{  
  /* Bildschirm loeschen */
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );  
  
  /* Auf Projektionsmodus umschalten */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0,0,display->getWidth(),display->getHeight());
  glOrtho(0,display->getWidth(),0,display->getHeight(),0,128);

  /* Zurueckschalten und Ansicht einstellen */
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();
  
/*  illustrator.drawRect(0.0,1.0,0.0,0.0,0.0,display.getWidth(),display.getHeight());*/
  illustrator->glPrint(50.0, 0.0, 0.0, 1.0, 10.0, display->getHeight()-100.0, name.c_str());
  SDL_GL_SwapBuffers();
}

void FileManager::handleEvents()
{
  SDL_Event event;
  
  while ( SDL_PollEvent( &event ) )
  {
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
      case SDL_VIDEORESIZE:
        /* Groesse vom Fenster geaendert */
        display->resizeWindow( event.resize.w, event.resize.h );
        break;
      case SDL_QUIT:
        exit(0);
        break;
      case SDL_KEYDOWN:
        if((event.key.keysym.sym >= 'a' && event.key.keysym.sym <= 'z') || (event.key.keysym.sym >= '0' && event.key.keysym.sym <= '9'))
        {
          name += toupper(event.key.keysym.sym);
        } else {
          switch(event.key.keysym.sym)
          {
            case SDLK_RETURN:
              doinput = false;
              break;
            case SDLK_ESCAPE:
              exit(0);
              break;
            default:
              break;
          }
        }
        break;
      default:
        break;
    }
  }
}