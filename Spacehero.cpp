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
#include "Spacehero.h"
#include <sys/time.h>
#include <unistd.h>

Spacehero::Spacehero(SpaceDisplay &d, Universe &u)
  : state(spacehero_edit), won(false), bflags(), editor(u),
    display(d), universe(u), paruni(0), view(SpaceDisplay::PutView)
{
}

Spacehero::SpaceheroState Spacehero::play(SpaceDisplay::BridgeView myview)
{ 
  view = myview; 
  switch(view)
  {
   case SpaceDisplay::EditorView:
      state = spacehero_starteditor;
      break;
    case SpaceDisplay::IntroView:
    case SpaceDisplay::MenuView:
      paruni = &universe;
      state = spacehero_simulate;
      break;
    case SpaceDisplay::SimulationView:
      state = spacehero_startsimu;
      break;
    case SpaceDisplay::PutView:
    default:
      state = spacehero_edit;
      break;
  }
  
  while (true)
  {
    if(bflags.checkFlag(ButtonFlags::exit))
    {
      state = spacehero_exit;
    }

    switch (state)
    {
      case spacehero_starteditor:
	view = SpaceDisplay::EditorView;
        editor.setAllowAll(true);
        state = spacehero_edit;
        break;
      case spacehero_stopeditor:
        editor.setAllowAll(false);
        state = spacehero_edit;
        break;
      case spacehero_edit:
        state = edit();
        break;
      case spacehero_startsimu:
        paruni = new Universe(universe);
        paruni->tinit();
        state = spacehero_simulate;
        break;
      case spacehero_simulate:
        state = simulate();
        break;
      case spacehero_stopsimu:
        delete(paruni);
        state = spacehero_edit;
        break;
      case spacehero_next:
      case spacehero_exit:
      case spacehero_emptyEditor:
        return state;
        //break;
      default:
        std::cerr << "unreachable state" << std::endl;
        return state;
    }
  }
}

Spacehero::SpaceheroState Spacehero::edit()
{
  editor.parseButtons(bflags);

  if(bflags.checkFlag(ButtonFlags::startSimulation))
  {
    state = spacehero_startsimu;
  }

  if(bflags.checkFlag(ButtonFlags::startEditor))
  {
    state = spacehero_starteditor;
  }
  
  if(bflags.checkFlag(ButtonFlags::saveLevel))
  {
    FileManager saveas;
    std::string savefile = "/tmp/"+saveas.getFile(display,universe)+".txt";
    std::cout << "Wird jetzt gespeichert in: " << savefile << std::endl;
    std::ofstream levelwrite(savefile.c_str());
    levelwrite << universe;
    //state = ?
  }

  if(bflags.checkFlag(ButtonFlags::skipLevel))
  {
    return spacehero_next;
  }

  display.drawBridge(universe,editor.getView(),editor.getQuotient(),editor.getHoleWeight());
  display.handleEvents(editor.getView(), bflags, editor);
  editor.drawMouse(&display);
  SDL_GL_SwapBuffers();
  
  return state;
}

#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)<(b))?(b):(a))
Spacehero::SpaceheroState Spacehero::simulate()
{
  paruni->tick();
  paruni->move(maxframerate);

  display.handleEvents(view, bflags, editor);

  if(bflags.checkFlag(ButtonFlags::breakSimulation))
  {
    state = spacehero_edit;
  }

  if(bflags.checkFlag(ButtonFlags::replaySimulation))
  {
    state = spacehero_startsimu;
  }

  if(view == SpaceDisplay::IntroView)
  {
    double menutime;
    int j;

    display.getDisplay()->cleanDisplay();
    display.getDisplay()->OrthoMode();

    menutime = paruni->elapsed();

    if(menutime > 12.15)
    {
      paruni->galaxies.at(0).setexists(false);
    }

    if(bflags.viewFlag(ButtonFlags::breakIntro))
    {
      display.showMenu(100);
    } else if(menutime > 16)
    {
      display.showMenu(menutime-16);
    }
       
    for(j = 1; j < exp(-1.1*pow((menutime-15),2))*40; j++) 
    {
      paruni->stars[(int)((rand() / (RAND_MAX + 1.0))*paruni->stars.size())].vz = (10e5*(rand() / (RAND_MAX + 1.0)))+2e5;
    }

    display.displayUniverse(*paruni, (*display.getDisplay()).getWidth(), (*display.getDisplay()).getHeight());     
  } 
  else if(view == SpaceDisplay::MenuView)
  {
    display.getDisplay()->cleanDisplay();
    display.showMenu(100);
  }
  else
  {
    display.drawBridge(*paruni,SpaceDisplay::SimulationView,(paruni->getmaxtime()-paruni->elapsed())/paruni->getmaxtime());
  }

  SDL_GL_SwapBuffers();

  // ZEIT verballern
  float sle = 1.0e6*max(0.0,maxframerate - paruni->ldelta());
  useconds_t sleep = 1.0e6*max(0.0,maxframerate - paruni->ldelta());
  if(0 != usleep(sleep)) std::cerr << "usleep failed with " << sleep << " ns and " << maxframerate << " maxframerate and ldelta " << paruni->ldelta() << " and sle " << sle << std::endl;
  
  if(view == SpaceDisplay::IntroView || view == SpaceDisplay::MenuView)
  {
    if(bflags.checkFlag(ButtonFlags::startGame))
    {
      return spacehero_next;
    }

    if(bflags.checkFlag(ButtonFlags::exit))
    {
      return spacehero_exit;
    }

    if(bflags.checkFlag(ButtonFlags::startEditor))
    {
      return spacehero_emptyEditor;
    }
  }
  else
  {
    if(paruni->timeout())
    {
      display.showEnd(false,bflags);
      if(bflags.checkFlag(ButtonFlags::replaySimulation))
      { 
	return spacehero_startsimu;
      }
      return spacehero_edit;
    }

    if((won = paruni->won()))
    {
      display.showEnd(true,bflags);
      if(bflags.checkFlag(ButtonFlags::replaySimulation))
      {
	return spacehero_startsimu;
      }
      else if(editor.isAllowAll())
      {
	return spacehero_edit;
      } else {
	return spacehero_next;
      }
    }
  }

  paruni->tack();
  return state;
}

Spacehero::SpaceheroState Spacehero::handleEvents() {
  return state;
}


#if 0
Kamera cam;

int win, simulationTime, i, first;
int clocktime;
float diff;

state.mediumHole(this->state);

/* Solange nicht beendet wurde, oder das Level geschafft */  
win = 0;
first = 1;
while(win == 0)
{

  state.clearStatus();

  /* Fenster zeichnen */
  uni.drawPut( *this );

  /* so lange bis jemand die Simulation startet */
  if(first)
  {
    first = 0;
    continue;
  }

  while( !(state.m_startSimulation || state.m_exit) )
  { 
    handleEvents( *this, PUT, uni );
  }

  if(state.m_exit) break;

  /* Simulation */
  do
  {
    state.m_replaySimulation = 0;

    /* Paralleluniversum erzeugen */
    paruni.goal = uni.goal;
    paruni.holesSize = uni.holesSize;
    paruni.galaxiesSize = uni.galaxiesSize;
    paruni.starsSize = uni.starsSize;
    paruni.stars = (skymass*)malloc(sizeof(skymass)*paruni.starsSize);

    /* Inputarrays kopieren */
    for(i = 0; i < uni.galaxiesSize; i++)
    {
      paruni.galaxies[i] = uni.galaxies[i];
    }

    for(i = 0; i < uni.holesSize; i++)
    {
      paruni.holes[i] = uni.holes[i];
    }

    for(i = 0; i < uni.starsSize; i++)
    {
      paruni.stars[i] = uni.stars[i];
    }

    cam.rx = cam.ry = cam.rz = 0;

    /* bis die Zeit abgelaufen ist, oder Ziel erreicht */
    for(simulationTime = 1; simulationTime < MAXTIME; simulationTime++)
    {
      clocktime = clock();
      cam.rx = 0;

      paruni.move(simulationTime);
      paruni.eventHorizon();

      paruni.drawSimulation(*this, &cam, simulationTime);

      handleEvents( *this, SIMULATION, uni );

      /* Abbrechen */
      if(state.m_breakSimulation || state.m_replaySimulation || state.m_exit)
      {
        simulationTime = MAXTIME;
        break;
      }

      /* gewonnen? */
      if(sqrt(pow(paruni.galaxies[0].x - paruni.goal.x,2)+pow(paruni.galaxies[0].y - paruni.goal.y,2)) < paruni.goal.r && paruni.galaxies[0].exists)
      {
        break;
      }

      /* etwas warten... */
      diff = TIMESTEP-((float)(clock()-clocktime)/CLOCKS_PER_SEC);
      diff = (diff > 0)?diff:0;
      usleep(1000000*diff);
    }

    glEnable(GL_BLEND);

    if(simulationTime == MAXTIME)
    {
      /* TIMEOUT oder ABBRUCH */
      win = 0;

      if(!(state.m_breakSimulation || state.m_replaySimulation || state.m_exit))
      {
        putImage( IMG_LOST, 0.5-((265.0/102)*0.12)*0.5, 0.5-(0.12*0.5), (265.0/102)*0.12, 0.12);
        SDL_GL_SwapBuffers();
        for(i = 0; i < 200; i++)
        {
          handleEvents( *this, SIMULATION, uni );
          if(state.m_breakSimulation || state.m_replaySimulation || state.m_exit)
          {
            break;
          }
          usleep(10000);
        }
      }
    } else {
      /* WIN */
      win = 1;
      putImage( IMG_WIN, 0.5-((629.0/102)*0.12)*0.5, 0.5-(0.12*0.5), (629.0/102)*0.12, 0.12 );
      SDL_GL_SwapBuffers();
      for(i = 0; i < 500; i++)
      {
        handleEvents( *this, SIMULATION, uni );
        if(state.m_breakSimulation || state.m_replaySimulation || state.m_exit)
        {
          break;
        }
        usleep(10000);
      }
    }

    glDisable(GL_BLEND);

    free(paruni.stars);      
  } while(state.m_replaySimulation);

  paruni.stars = NULL;
  free(uni.stars);

  if(state.m_exit) break;
}
}
#endif


