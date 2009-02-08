#if 0
#include "handleEvents.h"
#include "display.h"

void handleEvents(GLdisplay &display, int part, Universe &uni)
{
  int i, j, remove;

  GLdouble modelMatrix[16];
  GLdouble projMatrix[16];
  int viewport[4];
  double mousex, mousey, mousez, zpos;

  skymass newHole;
  newHole.z = 0;
  newHole.vx = 0;
  newHole.vy = 0;
  newHole.vz = 0;
  newHole.mass = 1e10;
  newHole.inLevel = 0;

  while ( SDL_PollEvent( &display.event ) )
  {
    switch( display.event.type )
    {
      case SDL_ACTIVEEVENT:
        /* Fokus verloren (z.B. minimiert) */
        if ( display.event.active.gain == 0 )
        {
          display.isActive = 0;
        }
        else
        {
          display.isActive = 1;
        }  
      case SDL_MOUSEBUTTONDOWN:
        /* Buttons */
        if(part == PUT || part == SIMULATION)
        {
          display.checkButtons();
        }
      
        /* Nur fuer Setzfenster */
        if(part == PUT)
        {        
          /* schwarzes Loch setzen */
          if(display.event.motion.x > UNIVERSE_LEFT && 
             display.event.motion.x < display.width-(UNIVERSE_RIGHT+UNIVERSE_LEFT) && 
             display.event.motion.y > UNIVERSE_TOP && 
             display.event.motion.y < display.height-(UNIVERSE_TOP+UNIVERSE_BOTTOM)
            )
          {
            remove = 0;
                        
            /* Mausposition umrechnen */
            glGetIntegerv(GL_VIEWPORT,viewport);
            glGetDoublev(GL_PROJECTION_MATRIX,projMatrix);
            glGetDoublev(GL_MODELVIEW_MATRIX,modelMatrix);
            glReadPixels(display.event.motion.x, display.event.motion.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &zpos );
            gluUnProject(display.event.motion.x, display.event.motion.y, zpos,
                modelMatrix, projMatrix, viewport,
                &mousex, &mousey, &mousez
            );
            
            mousey = 1.0-mousey;
            
            for(i = 0; i < uni.holesSize; i++)
            {
              if( (pow(mousex-uni.holes[i].x,2)+pow(mousey-uni.holes[i].y,2))
                    <= pow(HOLESIZE*(sqrt(uni.holes[i].mass/HOLEMEDIUMMASS)),2) )
              {
                remove = 1;
                
                /* pruefen ob der ueberhaupt geloescht werden darf */
                if(!uni.holes[i].inLevel)
                {
                  uni.massreserve += uni.holes[i].mass;

                  /* die danach nachruecken lassen */
                  for(j = i; j < (uni.holesSize-1); j++)
                  {
                    uni.holes[j] = uni.holes[j+1];
                  }
                  uni.holesSize--;
                }
              }
            }
            
            if(!remove && uni.massreserve >= display.state.m_holeWeight)
            {
              newHole.x = mousex;
              newHole.y = mousey;
              newHole.mass = display.state.m_holeWeight;
              uni.massreserve -= newHole.mass;
              uni.holes[uni.holesSize++] = newHole;
            }
            
          }
          uni.drawPut( display );
        }
        break;
      case SDL_VIDEORESIZE:
        /* Groesse vom Fenster geaendert */
        display.resizeWindow( display.event.resize.w, display.event.resize.h );
        
        if(part == PUT)
        {
          uni.drawPut( display );
        }
        /* Simulation wird sowieso gleich wieder gezeichnet */
        break;
      case SDL_QUIT:
        free(uni.stars);
        // exitApp( ); XXX is now implicit constructor call
	exit(0);
        break;
      case SDL_KEYDOWN:
        switch(display.event.key.keysym.sym)
        {
          case SDLK_ESCAPE:
            free(uni.stars);
            //exitApp( display );
	exit(0);
            break;
          case SDLK_SPACE:
            display.state.m_breakIntro = 1;
            break;
          default:
            break;
        }
        break;
      default:
        break;
    }
  }
}
#endif
