#include "game.h"

/* laed ein Level */
/*void loadLevel(int level, skymass galaxies[], int *galaxiesSize, skymass holes[], int *holesSize, skygoal *goal)
{
  holes[0].x = 0.2;
  holes[0].y = 0.8;
  holes[0].z = 0;
  holes[0].vx = 0;
  holes[0].vy = 0;
  holes[0].vz = 0;
  holes[0].mass = 2e12;
  holes[0].inLevel = 1;

  holes[1].x = 0.8;
  holes[1].y = 0.2;
  holes[1].z = 0;
  holes[1].vx = 0;
  holes[1].vy = 0;
  holes[1].vz = 0;
  holes[1].mass = 3e12;
  holes[1].inLevel = 1;
  
  holes[2].x = 0.8;
  holes[2].y = 0.8;
  holes[2].z = 0;
  holes[2].vx = 0;
  holes[2].vy = 0;
  holes[2].vz = 0;
  holes[2].mass = 1e12;
  holes[2].inLevel = 1;
  
  galaxies[0].x = 0.1;
  galaxies[0].y = 0.5;
  galaxies[0].z = 0;
  galaxies[0].vx = 300e3;
  galaxies[0].vy = 90e3;
  galaxies[0].vz = 0.0;
  galaxies[0].mass = 1.73e11;
  galaxies[0].inLevel = 1;
  galaxies[0].exists = 1;
  
  galaxies[1].x = 0.9;
  galaxies[1].y = 0.46;
  galaxies[1].z = 0;
  galaxies[1].vx = -300e3;
  galaxies[1].vy = 90e3;
  galaxies[1].vz = 0.0;
  galaxies[1].mass = 1.73e11;
  galaxies[1].inLevel = 1;
  galaxies[1].exists = 1;

  if(*holesSize < 3)
  {
    (*holesSize) = 3;
  }
  
  if(*galaxiesSize < 2)
  {
    (*galaxiesSize) = 2;
  }
  
  goal->x = 0.8;
  goal->y = 0.6;
  goal->z = 0;
  goal->r = 0.1;
}*/

void startRound(GLdisplay *display, char *level)
{
  Universe uni, paruni;
  Kamera cam;
  int win, simulationTime, i, first;
  int clocktime;
  float diff;

  uni.holesSize = 0;
  uni.galaxiesSize = 0;
  uni.starsSize = 0;
  uni.massreserve = MAXSTARTRESERVE;
  
  /* Level laden */
  levelladen(level, uni.galaxies, &uni.galaxiesSize, uni.holes, &uni.holesSize, &uni.goal);
  BUT_mediumHole(&display->state);

  /* Solange nicht beendet wurde, oder das Level geschafft */  
  win = 0;
  first = 1;
  while(win == 0)
  {
    /* Galaxie erzeugen */
    uni.starsSize = 0;
    uni.stars = NULL;
    for(i = 0; i < uni.galaxiesSize; i++)
    {
      constructGalaxy(&uni.galaxies[i], &uni.stars, &uni.starsSize);
    }

    clearStatus(&display->state);

    /* Fenster zeichnen */
    drawPut( display, &uni );

    /* so lange bis jemand die Simulation startet */
    if(first)
    {
      first = 0;
      continue;
    }
    
    while( !(display->state.startSimulation || display->state.exit) )
    { 
      handleEvents( display, PUT, &uni );
    }

    if(display->state.exit) break;

    /* Simulation */
    do
    {
      display->state.replaySimulation = 0;
      
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

        move(paruni.galaxies, paruni.galaxiesSize, paruni.holes, paruni.holesSize, paruni.stars, paruni.starsSize, simulationTime);
        eventHorizon(paruni.stars, paruni.starsSize, paruni.holes, paruni.holesSize, paruni.galaxies, paruni.galaxiesSize);

        drawSimulation(display, &paruni, &cam, simulationTime);
        
        handleEvents( display, SIMULATION, &uni );
        
        /* Abbrechen */
        if(display->state.breakSimulation || display->state.replaySimulation || display->state.exit)
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

        if(!(display->state.breakSimulation || display->state.replaySimulation || display->state.exit))
        {
          putImage( IMG_LOST, 0.5-((265.0/102)*0.12)*0.5, 0.5-(0.12*0.5), (265.0/102)*0.12, 0.12, display );
          SDL_GL_SwapBuffers();
          for(i = 0; i < 200; i++)
          {
            handleEvents( display, SIMULATION, &uni );
            if(display->state.breakSimulation || display->state.replaySimulation || display->state.exit)
            {
              break;
            }
            usleep(10000);
          }
        }
      } else {
        /* WIN */
        win = 1;
        putImage( IMG_WIN, 0.5-((629.0/102)*0.12)*0.5, 0.5-(0.12*0.5), (629.0/102)*0.12, 0.12, display );
        SDL_GL_SwapBuffers();
        for(i = 0; i < 500; i++)
        {
          handleEvents( display, SIMULATION, &uni );
          if(display->state.breakSimulation || display->state.replaySimulation || display->state.exit)
          {
            break;
          }
          usleep(10000);
        }
      }

      glDisable(GL_BLEND);

      free(paruni.stars);      
    } while(display->state.replaySimulation);

    paruni.stars = NULL;
    free(uni.stars);
    
    if(display->state.exit) break;
  }
}
