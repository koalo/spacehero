#include "spacehero.h"

#include "displayAbstract.h"
#include "displaySpacehero.h"
#include <unistd.h>
#include <time.h>
#include "galaxy.h"
#include "game.h"

#define VORN 300
#define SPEED 0

#define LEVEL 3

void intro(GLdisplay *display)
{
  Universe uni;
  Kamera cam;
  int simulationTime, i, j;
  int clocktime;
  float diff, herotime;
  skymass newstar;

  double scale;

  /* Zufall starten */
  srand((unsigned int) time(NULL));

  uni.holesSize = 0;
  uni.galaxiesSize = 2;
  uni.starsSize = 0;

  uni.galaxies[0].x = 0.1;
  uni.galaxies[0].y = 0.1;
  uni.galaxies[0].z = VORN;
  uni.galaxies[0].vx = 100e3;
  uni.galaxies[0].vy = 100e3;
  uni.galaxies[0].vz = SPEED;
  uni.galaxies[0].mass = 4.3e11;
  uni.galaxies[0].inLevel = 0;
  uni.galaxies[0].exists = 1;
  uni.galaxies[0].nograv = 1;
  
  uni.galaxies[1].x = 1.42;
  uni.galaxies[1].y = 1.42;
  uni.galaxies[1].z = VORN;
  uni.galaxies[1].vx = -200e3;
  uni.galaxies[1].vy = -200e3;
  uni.galaxies[1].vz = SPEED;
  uni.galaxies[1].mass = 20e11;
  uni.galaxies[1].inLevel = 1;
  uni.galaxies[1].exists = 1;
  uni.galaxies[1].nograv = 1;
  
  /* Galaxie erzeugen */
  uni.starsSize = 0;
  uni.stars = NULL;
  constructGalaxy(&uni.galaxies[0], &uni.stars, &uni.starsSize);  
  constructGalaxy(&uni.galaxies[1], &uni.stars, &uni.starsSize);

  for(j = 0; j < uni.starsSize; j++)
  {
    uni.stars[j].z = VORN;
    uni.stars[j].vz = SPEED;
  }

  cam.rx = cam.ry = cam.rz = 0;

  /* bis die Zeit abgelaufen ist, oder Ziel erreicht */
  display->state.breakIntro = 0;
  for(simulationTime = 1; simulationTime < 1200; simulationTime++)
  {
    clocktime = clock();
    cam.rx = 30;

    move(uni.galaxies, uni.galaxiesSize, uni.holes, uni.holesSize, uni.stars, uni.starsSize, simulationTime);
 
    /* Fenster und Tiefenbuffer loeschen */
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
/*   if(simulationTime > 500)
    {
      for(j = 0; j < 8; j++) uni.stars[(int)((rand() / (RAND_MAX + 1.0))*uni.starsSize)].vz = (10e8*(rand() / (RAND_MAX + 1.0)))+2e8;
    }
  */  
    /* Universum zeichnen */ 
    if(simulationTime > 4)
    {
      displayUniverse(display, &uni, PERSPECTIVE, display->width, display->height);
      scale = 0.9;
      herotime = 1.0-exp(-(simulationTime-600)*0.005);
      herotime = (herotime > 0)?herotime:0;
      herotime = (herotime < scale)?herotime:scale;
      
      putImage( IMG_SPACEHERO, 0.5-scale*herotime/2, 0.5-(scale*(186.0/634.0)*herotime)/2.0, scale*herotime, scale*(186.0/634.0)*herotime, display );
    }

    /* Versteckten Buffer aktivieren */
    SDL_GL_SwapBuffers();
 
    handleEvents( display, 457645, &uni );
     
    if(display->state.breakIntro) break;

    /* etwas warten... */
    diff = TIMESTEP-((float)(clock()-clocktime)/CLOCKS_PER_SEC);
    diff = (diff > 0)?diff:0;
    usleep(1000000*diff);
  }

  free(uni.stars);
}


int main(int argc, char *argv[])
{ 
  GLdisplay display;
  char levelString[50];
  int rlevel = -1, altrlevel;

  /* Fenster initialisieren */
  initDisplay(&display);

  /* Zufall starten */
  srand((unsigned int) time(NULL));

  SDL_ShowCursor(SDL_DISABLE);
  intro(&display);
  SDL_ShowCursor(SDL_ENABLE);

  do
  {
    if(argc == 1)
    {
      altrlevel = rlevel;
      do
      {
        rlevel = (int)((rand() / (RAND_MAX + 1.0))*LEVEL)+1;
      } while(rlevel == altrlevel);
      
      sprintf(levelString,"level%i",rlevel);
    } else {
      strcpy(levelString,argv[1]);
    }
  
    startRound(&display,levelString);
  } while(rlevel != -1 && !display.state.exit);
  
  exitApp(&display);
  return 0;
}

void displayMenu()
{

}

void gameOver(int level, int score)
{

}


