#include "Universe.h"

#include "display.h"
#include "GLdisplay.h"
#include "BStatus.h"

#include "displayUniverse.h"


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
  seed=5;
  return std::vector<Star>();
#if 0
  /*
     skymass star;
     double Stars_Angle;
     double R_Array[ORBITS_MAX],
     Orbit_Velocity[ORBITS_MAX], first_angle;


     int orbits1, i , j, s, Stars_Amount[ORBITS_MAX], starsAmount, starsAmountOrbit, realstarsAmount;
     skymass *stars;
     */

  /* Anzahl von Orbits, von Masse abhaengig (Orbits MIN = 5, MAX = 15) */
  int orbits1 = (mass * 2) / 4e10;
  if(orbits1 > ORBITS_MAX)
  {
    printf("Galaxie ist zu schwer!\n");
    exit(1);
  }
  int starsAmount = (mass) / 4e8;

  std::vector<double> R_Array(ORBITS_MAX);
  /* R_Array[20] <- Array von Radius der Orbit fuer 2 Galaxien */
  /* 1 Radius zufaellige Radius berechnen (incl. R_MIN_CENTER) */
  R_Array[0] = R_MIN_CENTER + (double)((rand() / (RAND_MAX + 1.0)) / 150.0);

  /* Naechste Radius Berechnen (incl. R_MIN) */

  for (i = 1; i < orbits1; i++)
  {
    R_Array[i] = R_Array[i-1] + R_MIN +  ((double)((rand() / (RAND_MAX + 1.0))) / 550.0);
  }


  /*  starsAmountOrbit = starsAmount / orbits1;*/
  /* das kann weg, brauch ich Stars_amount[i] nicht mehr*/
  int realstarsAmount = 0;
  std::vector<double> Stars_Amount(ORBITS_MAX);
  for(i = 0; i < orbits1; i++)
  {  
    Stars_Amount[i] = starsAmount / (orbits1);
    realstarsAmount += Stars_Amount[i];
  }

  int starsAmountOrbit = starsAmount / orbits1;

  /* starsArrayGroesse festlegen */
  /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

  /*
   *INstars = (skymass*)realloc(*INstars, sizeof(skymass) * (realstarsAmount + *starsSize));
   if(*INstars == NULL)
   {
   printf("Kein Arbeitsspeicher fuer die Sterne!\n");
   exit(1);
   }
   stars = &(*INstars)[*starsSize];
   */


  /* X, Y Koordiante fuer jede Stern ausrechnen */

  /* Erste Winkel (first_angle) zwischen y=0,+x und Erste Stern auf der Bahn zufaellig,
   * dann jede naechste Wikel durch Drewinkel Stars_Angle erzeugt
   * Erzeugung durch Drehmatrix:
   * [x2] = (cos&  -sin&) [x1]
   * [y2]   (sin&   cos&) [y1]
   *
   * dazu star.vx, star.vy <- normierte GeschwindigkeitsVektoren von Stern senkrecht zu R, entgegengesetzt zu Zeit
   */

  /* fuer Galaxy1 */

  std::vector<Star> stars(realstarsAmount);

  s = -1;
  for(i = 0; i < orbits1; i++)
  {
    s++;

    first_angle = (double)(360 * (rand() / (RAND_MAX + 1.0)));

    star.x = (R_Array[i] * cos(first_angle * M_PI/180)) - (0.0 * sin(first_angle * M_PI/180)) + galaxy->x;
    star.y = (R_Array[i] * sin(first_angle * M_PI/180)) + (0.0 * cos(first_angle * M_PI/180)) + galaxy->y;


    star.vx = (0 * cos(first_angle * M_PI/180)) - (1.0 * sin(first_angle * M_PI/180));
    star.vy = (0 * sin(first_angle * M_PI/180)) + (1.0 * cos(first_angle * M_PI/180));

    stars[s] = star;

    for(j = 1; j < (starsAmountOrbit); j++)
    {
      s++;

      Stars_Angle = (double)(360 * (rand() / (RAND_MAX + 1.0)));

      star.x = (stars[s-1].x - galaxy->x) * cos(Stars_Angle * M_PI/180) - (stars[s-1].y - galaxy->y) * sin(Stars_Angle * M_PI/180) + galaxy->x;

      star.y = (stars[s-1].x - galaxy->x) * sin(Stars_Angle * M_PI/180) + (stars[s-1].y - galaxy->y) * cos(Stars_Angle * M_PI/180) + galaxy->y;

      star.vx = (stars[s-1].vx * cos(Stars_Angle * M_PI/180)) - (stars[s-1].vy * sin(Stars_Angle * M_PI/180));
      star.vy = (stars[s-1].vx * sin(Stars_Angle * M_PI/180)) + (stars[s-1].vy * cos(Stars_Angle * M_PI/180));

      stars[s] = star;
    }

  }

  starsAmount = s + 1;
  *starsSize += starsAmount;

  /* Z Koordinate fuer jede Stern ausgeben */
  /* fuer 4 Gruppen Teilen */
  /* In der Mitte soll dichter sein ale am Randen */

  /* 1 Gruppe kriegt Z Koordinate von 0 bis 1/40 */
  for(i = 0; i < starsAmount; i=i+4)
  {
    stars[i].z = (double)((rand() / (RAND_MAX + 1.0)) / 100);
  }

  /* von 0 bis -1/40 */
  for(i = 1; i < starsAmount; i=i+4)
  {
    stars[i].z = - (double)((rand() / (RAND_MAX + 1.0)) / 100);
  }

  /* von 0 bis 1/30 */
  for(i = 2; i < starsAmount; i=i+4)
  {
    stars[i].z = (double)((rand() / (RAND_MAX + 1.0)) / 80);
  }

  /* von 0 bis -1/30 */
  for(i = 3; i < starsAmount; i=i+4)
  {
    stars[i].z = - (double)((rand() / (RAND_MAX + 1.0)) / 80);
  }

  /* Masse für jede Stern verteilen*/

  /* Massen fur Sternen die herum fliegen */
  for(i = 0; i < starsAmount; i++)
  {
    stars[i].mass = 1e3;
    stars[i].exists = 1;
  }

  /* Nuetige AnfangsGeschwindigkeit 
   * Orbit_Velocity = sqrt((G*M)/R)
   *
   * und NewtonBeschleunigung von Sternen auf verschidenen Bahnen
   * Orbit_Accelerate = G*(M/R²)
   *
   * M - Masse von Stern in Mittelpunkt
   *GRAVKONST- Gravitationskonstante
   */

  for(i=0; i < orbits1; i++)
  {
    Orbit_Velocity[i] = sqrt((GRAVKONST * galaxy->mass * SUNMASS) / (R_Array[i] * WIDTHINMETERS));
  }



  /* VX und VY -Geschwindigkeiten, für jede Stern berechnen */

  s = -1;
  for(i = 0; i < orbits1; i++)
  { 

    for(j = 0; j < (starsAmountOrbit); j++)
    {
      s++;

      /* Normirte GeschwindigkeitsVektor * Orbit_Geschwindigkeit */
      stars[s].vx *= Orbit_Velocity[i];
      stars[s].vy *= Orbit_Velocity[i];

      /* Dazu noch AnfangsGeschwindigkeit von Galaxy */
      stars[s].vx += galaxy->vx;
      stars[s].vy += galaxy->vy;

    }

  }
}
#endif
}

Universe::Universe(Level &l) :
  Level(l)
{
  for(std::vector<Galaxy>::iterator i = galaxies.begin(); i != galaxies.end(); i++) {
    std::vector<Star> gstars = i->getStars(seed);
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
