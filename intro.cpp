#if 0
#include "Universe.h"
#include "intro.h"
#include "handleEvents.h"
#include "displayUniverse.h"

#include "local.h"

void intro(GLdisplay &display)
{
  Universe uni;
  Kamera cam;
  int simulationTime, j;
  int clocktime;
  float diff, herotime;

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
  display.state.m_breakIntro = 0;
  for(simulationTime = 1; simulationTime < 1200; simulationTime++)
  {
    clocktime = clock();
    cam.rx = 30;

    uni.move(simulationTime);
 
    /* Fenster und Tiefenbuffer loeschen */
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    if(simulationTime > 500)
    {
      for(j = 0; j < 8; j++) uni.stars[(int)((rand() / (RAND_MAX + 1.0))*uni.starsSize)].vz = (10e8*(rand() / (RAND_MAX + 1.0)))+2e8;
    }
    
    if(simulationTime == 570)
    {
      uni.galaxies[0].exists = 0;
    }
    
    /* Universum zeichnen */ 
    if(simulationTime > 4)
    {
      displayUniverse(display, uni, PERSPECTIVE,display.width,display.height);
      scale = 0.9;
      herotime = 1.0-exp(-(simulationTime-600)*0.005);
      herotime = (herotime > 0)?herotime:0;
      herotime = (herotime < scale)?herotime:scale;
      
      display.putImage( GLdisplay::IMG_SPACEHERO, 0.5-scale*herotime/2, 0.5-(scale*(186.0/634.0)*herotime)/2.0, scale*herotime, scale*(186.0/634.0)*herotime );
    }

    /* Versteckten Buffer aktivieren */
    SDL_GL_SwapBuffers();
 
    handleEvents( display, 457645, uni );
     
    if(display.state.m_breakIntro) break;

    /* etwas warten... */
    diff = TIMESTEP-((float)(clock()-clocktime)/CLOCKS_PER_SEC);
    diff = (diff > 0)?diff:0;
    usleep(1000000*diff);
  }

  free(uni.stars);
}


#endif
