#include "Universe.h"

#include "display.h"
#include "GLdisplay.h"
#include "BStatus.h"

#include "displayUniverse.h"

#include "local.h"

Star::Star(double R, double phi, double z, double v, double mass)
{
  x = R * cos(phi * M_PI/180);
  y = R * sin(phi * M_PI/180);
  z = z;

  vx = v * sin(phi * M_PI/180);
  vy = v * cos(phi * M_PI/180);
  vz = 0;

  this->mass = mass;
}


Level::Level(std::ifstream &in)
{
  goal = Goal(in);
  while(in.good()) {
    char c;
    in >> c;
    std::cerr << "Type: " << c << std::endl;
    switch(c) {
      case 'G': 
        galaxies.push_back( Galaxy(in) );
        break;
      case 'H':
        holes.push_back( Blackhole(in) );
        break;
      case 'S':
        in >> seed;
        break;
    }
  }
}

std::vector<Star> Galaxy::getStars(int seed) {
  const double NStars = mass / 4e8;
  const double NOrbits = mass / 4e10;
  const double NStarsPerOrbit = NStars/NOrbits;
  std::vector<Star> accu;

  srand(seed);

  int n=NOrbits;
  double orb = R_MIN_CENTER + (double)((rand() / (RAND_MAX + 1.0)) / 150.0);
  while (n--) {
    orb += R_MIN +  ((double)((rand() / (RAND_MAX + 1.0))) / 550.0);
    double v = sqrt((GRAVKONST * mass * SUNMASS) / (orb * WIDTHINMETERS));
    int m=NStarsPerOrbit;
    while (m--) {
      double phi = (double)(360 * (rand() / (RAND_MAX + 1.0)));
      double z = (double)((1.0/10.0) * (rand() / (RAND_MAX + 1.0)))-(1.0/20.0);
      accu.push_back(Star(orb,phi,z,v));
    }
  }
  return accu;
}

Universe::Universe(Level &l) :
  Level(l)
{
  for(std::vector<Galaxy>::iterator i = galaxies.begin(); i != galaxies.end(); i++) {
    std::vector<Star> gstars = i->getStars(seed);
    std::cerr << "got " << gstars.size() << " stars" << std::endl;
    copy(gstars.begin(),gstars.end(),back_inserter(stars));
  }
}


#if 0




skymass::skymass(const skymass& p):
  x(p.x),y(p.y),z(p.z),
  vx(p.vx),
  vy(p.vy),
  vz(p.vz),
  fx(p.fx),
  fy(p.fy),
  fz(p.fz),
  mass(p.mass),
  inLevel(p.inLevel),
  exists(p.exists),
  nograv(p.nograv)
{}


void Universe::move(int time)
{
  int i, j, k, teiler;/* half, full, max;

                         full = ((starsSize+1)*starsSize)/2;
                         half = full/2;

                         if(time % 2)
                         {
                         sum = 0;
                         max = half;
                         } else {
                         sum = half;
                         max = full;
                         }

                         for(i = 0; sum < max; sum+=i++)*/

  time = 5; /* nur Unsinn */

  teiler = 1;
  /*
     if(time%teiler == 0)
     {*/
  for(i = 0; i < starsSize; i++)
  {
    if(stars[i].exists && stars[i].nograv != 1) 
    {
      /*      for(j = i+1; j < starsSize; j++)
              {
              if(stars[j].exists)
              {
              applyNewton(&stars[i], &stars[j], teiler);
              }
              }*/

      for(k = 0; k < holesSize; k++)
      {
        applyNewton(&stars[i], &holes[k], teiler);
      }

      for(j = 0; j < galaxiesSize; j++)
      {
        applyNewton(&galaxies[j], &stars[i], teiler);
      } 
    }
  }

  for(i = 0; i < galaxiesSize; i++)
  {
    for(k = 0; k < holesSize; k++)
    {
      applyNewton(&galaxies[i], &holes[k], teiler);
    }    

    for(k = i+1; k < galaxiesSize; k++)
    {
      if(galaxies[i].exists && galaxies[k].exists && galaxies[i].nograv != 1 && galaxies[k].nograv != 1)
      {
        applyNewton(&galaxies[i], &galaxies[k], teiler);
      }
    }
  }




  /*}*/

  /* Bewegung fuer Sternen */
  for(i = 0; i < starsSize; i++)
  {
    if(stars[i].exists)
    {
      stars[i].x += (stars[i].vx/WIDTHINMETERS)*TIMESCALE;
      stars[i].y += (stars[i].vy/WIDTHINMETERS)*TIMESCALE;
      stars[i].z += (stars[i].vz/WIDTHINMETERS)*TIMESCALE;
    }
  }

  /* Bewegung fuer Koerper in MittelPunkte von Galaxien */
  for(i = 0; i < galaxiesSize; i++)
  {
    galaxies[i].x += (galaxies[i].vx/WIDTHINMETERS)*TIMESCALE;
    galaxies[i].y += (galaxies[i].vy/WIDTHINMETERS)*TIMESCALE;
    galaxies[i].z += (galaxies[i].vz/WIDTHINMETERS)*TIMESCALE;
  }

  /* Bewegung von Black Holes */
  /*for(i = 0; i < holesSize; i++)
    {
    holes[i].x += (holes[i].vx/WIDTHINMETERS)*TIMESCALE;
    holes[i].y += (holes[i].vy/WIDTHINMETERS)*TIMESCALE;
    }*/
}


void Universe::drawBridge(GLdisplay &display, int projection, int time)
{
  double center;
  int mrx, mry, y, i;
  double mrangle, curse;
  double width, height;

  /* Bildschirm loeschen */
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  width = display.width-(UNIVERSE_RIGHT+UNIVERSE_LEFT);
  height = display.height-(UNIVERSE_TOP+UNIVERSE_BOTTOM);

  /* Auf Projektionsmodus umschalten */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0,0,display.width,display.height);

  /* Projektion einstellen */
  /* blending */
  glDisable( GL_BLEND );
  glOrtho(0,display.width,0,display.height,0,128);  

  /* Zurueckschalten und Ansicht zuruecksetzen */
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();

  glScalef(1.0f, -1.0f, 1.0f);
  glTranslatef(0.0f, -display.height, 0.0f);

  /* INFOTEXT */
  /*
     1.0  PI -> 0.75 PI
     0.5  PI -> 0.0  PI
     0.0  PI -> 0.5  PI
     -0.5 PI -> 1.0  PI

     (x-0.25*PI)
     (x-0.5*PI)
     (x+0.5*PI)
     (x+1.5*PI)
     */  
  y = 0; 
  display.glPrint( TEXTR, TEXTG, TEXTB, 0.0f, TEXTSPACE*(y++), "Task: Navigate the green galaxy into the green target area.");
  for(i = 0; i < galaxiesSize; i++)
  {
    if(galaxies[i].exists)
    {
      curse = atan2(galaxies[i].vx,-galaxies[i].vy); /* Vertauscht und VZ geaendert, dadurch quasi acot2 */
      curse = (curse < 0)?curse+2*M_PI:curse;
      curse = curse*(180/M_PI);
      display.glPrint( TEXTR, TEXTG, TEXTB, 0.0f, TEXTSPACE*(y++), "%i. Galaxy: Mass: %.0e kg, Curse: %i°",(i+1),galaxies[0].mass,(int)round(curse));
    }
  }
  /* putImage( IMG_BACKGROUND, 0, 0, display->width, display->height, display );*/
  /*  drawRect( 0.0, 0.0, 0.0, UNIVERSE_LEFT, UNIVERSE_TOP, width, height );*/
  display.putImage( (projection==ORTHOGONAL)?GLdisplay::IMG_PANEL_MASS:GLdisplay::IMG_PANEL_TIME, display.width-UNIVERSE_RIGHT, 0, UNIVERSE_RIGHT, display.height);

  center = display.width-UNIVERSE_RIGHT+(UNIVERSE_RIGHT/2.0);

  if(projection == ORTHOGONAL)
  {
    alignPutButtons(display);
    mrangle = (double)(massreserve)/(double)(MAXSTARTRESERVE);
  }

  if(projection == PERSPECTIVE)
  {
    alignSimulButtons(display);
    mrangle = (double)(MAXTIME-time)/(double)(MAXTIME);
  }

  glEnable(GL_POLYGON_SMOOTH);

  if(projection == PERSPECTIVE || projection == ORTHOGONAL)
  {
    /* Zeigerposition berechnen */
    mrangle = (mrangle < 0)?0:mrangle;
    mrangle = mrangle*80-40;
    mrx = - (-display.height*0.11 * sin(mrangle * M_PI/180));
    mry = (-display.height*0.11 * cos(mrangle * M_PI/180));

    glLineWidth(4);

    glColor3f(0,0,0);
    glBegin( GL_LINES );
    glVertex3f( center+mrx, display.height*0.28+mry, 0.0 );
    glVertex3f( center, display.height*0.28, 0.0 );
    glEnd();
    glColor3f(1,1,1);
  }

  display.drawButtons();

  glDisable(GL_POLYGON_SMOOTH);

  display.drawButtons();

  displayUniverse(display, *this,projection, width, height);

  /* Versteckten Buffer aktivieren */
  SDL_GL_SwapBuffers();
}


void Universe::alignSimulButtons(GLdisplay &display)
{
  double center;

  display.clearButtons();
  center = display.width-UNIVERSE_RIGHT+(UNIVERSE_RIGHT/2.0);

  /* Simulation stoppen */
  display.addButton(GLdisplay::IMG_STOP, center, display.height-UNIVERSE_BOTTOM-(START_BUTTON*1.2), START_BUTTON, BStatus::breakSimulation);

  /* Replay */
  display.addButton(GLdisplay::IMG_REPLAY, center+REPLAY_BUTTON, display.height-UNIVERSE_BOTTOM-(START_BUTTON*2.1)-REPLAY_BUTTON, REPLAY_BUTTON, BStatus::replaySimulation);

  /* Exit */
  display.addButton(GLdisplay::IMG_EXIT, display.width-2*EXIT_BUTTON, UNIVERSE_TOP+2*EXIT_BUTTON, EXIT_BUTTON, BStatus::exit);
}

void Universe::alignPutButtons(GLdisplay &display)
{
  double center, ypos, margin;

  display.clearButtons();
  center = display.width-UNIVERSE_RIGHT+(UNIVERSE_RIGHT/2.0);

  /* Simulation starten */
  display.addButton(GLdisplay::IMG_START, center, display.height-UNIVERSE_BOTTOM-(START_BUTTON*1.2), START_BUTTON, BStatus::startSimulation);

  /* Exit */
  display.addButton(GLdisplay::IMG_EXIT, display.width-2*EXIT_BUTTON, UNIVERSE_TOP+2*EXIT_BUTTON, EXIT_BUTTON, BStatus::exit);

  ypos = display.height*0.35;
  margin = UNIVERSE_RIGHT*0.3;

#define TEXTURE(size) ((display.state.m_holeWeight-(HOLESMALLMASS/2) < (size) && display.state.m_holeWeight+(HOLESMALLMASS/2) > (size))?GLdisplay::IMG_ACTIVE:GLdisplay::IMG_BUTTON) 
  display.addButton(TEXTURE(HOLESMALLMASS), center-margin, ypos, SMALL_HOLE, BStatus::smallHole);
  display.addButton(TEXTURE(HOLEMEDIUMMASS), center, ypos, MEDIUM_HOLE, BStatus::mediumHole);
  display.addButton(TEXTURE(HOLELARGEMASS), center+margin, ypos, LARGE_HOLE, BStatus::largeHole);
#undef TEXTURE
}


void Universe::eventHorizon()
{
  /* fuer jede Stern wird geprueft ob die sich zu nah zu Black Hole befindet */
  /* wenn der Abstand kleiner als r ist, dann soll die verschwinden -> stars[j].exist = 0 statt 1 */ 

  int i, j;
  double r, x, y;


  for(i = 0; i < holesSize; i++)
  {
    for(j = 0; j < starsSize; j++)
    {
      x = holes[i].x - stars[j].x;
      y = holes[i].y - stars[j].y;
      r = sqrt(x*x + y*y);
      if(r < HOLESIZE*0.5*sqrt(holes[i].mass/HOLEMEDIUMMASS))
      {
        stars[j].exists = 0;
      }
    }
    /*fuer MittelPunkten von Galxies auch */
    for(j = 0; j < galaxiesSize; j++)
    {
      x = holes[i].x - galaxies[j].x;
      y = holes[i].y - galaxies[j].y;
      r = sqrt(x*x + y*y);
      if(r < HOLESIZE*0.5*sqrt(holes[i].mass/HOLEMEDIUMMASS))
      {
        galaxies[j].exists = 0;
      }
    }

  }
}

void Universe::drawPut(GLdisplay &display)
{ 
  drawBridge(display, ORTHOGONAL, 0);
};
void Universe::drawSimulation( GLdisplay &display, Kamera *cam, int time )
{
  drawBridge(display, PERSPECTIVE, time);
  cam->rx = 0; /* nur Unsinn */
};
#endif
