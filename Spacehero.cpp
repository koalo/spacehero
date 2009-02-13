#include "Spacehero.h"
#include <sys/time.h>

Spacehero::Spacehero(SpaceDisplay &d, Universe &u)
  :display(d), universe(u)
{
  state = spacehero_edit;
}

bool Spacehero::play()
{
  ButtonFlags bflags;
  
  while (true) {
    display.handleEvents(SpaceDisplay::SimulationView, universe, bflags);
    if(bflags.checkFlag(ButtonFlags::breakSimulation)) state = spacehero_next;   
    switch (state) {
      case spacehero_edit:
        state = edit();
        break;
      case spacehero_startsimu:
        paruni = new Universe(universe);
        paruni->tick();
        state = spacehero_simulate;
        break;
      case spacehero_simulate:
        //std::cerr << i << std::endl;
        state = simulate();
        break;
      case spacehero_stopsimu:
        delete(paruni);
        state = spacehero_edit;
        break;
      case spacehero_next:
        return true;
        //break;
      case spacehero_exit:
        return false;
        //break;
      default:
        std::cerr << "unreachable state" << std::endl;
        return false;
    }
  }
}

Spacehero::SpaceheroState Spacehero::edit() {

  return spacehero_startsimu;
/*  display.drawEditor(universe);*/
  //return handleEvents(); // das ding muesste mir irgendwie mitteilen, was als naechstes passieren soll

}

#define min(a,b) (a)<(b)?(a):(b)
#define max(a,b) (a)<(b)?(b):(a)
Spacehero::SpaceheroState Spacehero::simulate() {
  double delta=paruni->delta();
  paruni->move(delta);

  display.drawBridge(*paruni,SpaceDisplay::SimulationView);

  paruni->tack();
  if (paruni->timeout()) return spacehero_next; // XXX
  if ((won = paruni->won())) return spacehero_next;

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


