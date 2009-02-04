#include "displaySpacehero.h"

#define TEXTSPACE 15
#define TEXTR 0.0f
#define TEXTG 1.0f
#define TEXTB 0.0f
  
#ifdef FAST
static inline 
#endif
void drawSkymass(skymass body, float size)
{
  /* Objekt zeichnen */
  glBegin(GL_QUADS);
    glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -size+body.x, -size+body.y, body.z );
    glTexCoord2f( 1.0f, 0.0f ); glVertex3f(  size+body.x, -size+body.y, body.z );
    glTexCoord2f( 1.0f, 1.0f ); glVertex3f(  size+body.x,  size+body.y, body.z );
    glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -size+body.x,  size+body.y, body.z );
  glEnd();
/*
  glBegin(GL_QUADS);
    glTexCoord2f( 0.0f, 0.0f ); glVertex3f( body.x, -size+body.y, -size+body.z );
    glTexCoord2f( 1.0f, 0.0f ); glVertex3f( body.x, -size+body.y, size+body.z );
    glTexCoord2f( 1.0f, 1.0f ); glVertex3f( body.x,  size+body.y, size+body.z );
    glTexCoord2f( 0.0f, 1.0f ); glVertex3f( body.x,  size+body.y, -size+body.z );
  glEnd();

  glBegin(GL_QUADS);
    glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -size+body.x, body.y, -size+body.z );
    glTexCoord2f( 1.0f, 0.0f ); glVertex3f( -size+body.x, body.y, size+body.z );
    glTexCoord2f( 1.0f, 1.0f ); glVertex3f( size+body.x,  body.y, size+body.z );
    glTexCoord2f( 0.0f, 1.0f ); glVertex3f( size+body.x,  body.y, -size+body.z );
  glEnd();*/
}

void displayUniverse( GLdisplay *display, Universe *uni, int projection, int width, int height )
{
  int i;
  GLfloat ratio;
  float zoomX, zoomY, viewrad;
  GLUquadricObj *pObj = gluNewQuadric();
GLfloat LightPosition[]= { 0.0f, 0.0f, 2.0f, 1.0f };
GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };	

  /* Universum zeichnen */
  /* blending */
  glBlendFunc( GL_SRC_ALPHA, GL_ONE );
  glEnable( GL_BLEND );

  /* Auf Projektionsmodus umschalten */
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  glViewport(UNIVERSE_LEFT,UNIVERSE_TOP,width,height);

  if(projection == ORTHOGONAL)
  {
    /* Projektion einstellen */
    gluOrtho2D(0,width,0,height);
  }

  if(projection == PERSPECTIVE)
  {
    /* Vor Division durch Null schuetzen */
    if (height == 0)
    {
      height = 1;
    }
   
    ratio = ( GLfloat )width / ( GLfloat )height;

    /* Perspektive einstellen */
    gluPerspective((float)VIEWANGLE, ratio, 0.0000001f, 10000000.0f);

    /* Zoom einstellen */
    viewrad = (VIEWANGLE/360.0)*(2*M_PI);
    zoomY = (height*0.5) / tan(viewrad/2.0);
    zoomX = (width*0.5) / tan((viewrad*ratio)/2.0);
    display->zoom = (zoomX > zoomY)?zoomX:zoomY;
  }

  /* Zurueckschalten und Ansicht zuruecksetzen */
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();

  if(height > width)
  { 
    glScalef(width, width, 1.0);
  } else {
    glScalef(height, height, 1.0);
  }

  glScalef(1.0f, -1.0f, 1.0f);

  if(projection == ORTHOGONAL)
  {
    glTranslatef(0.0f, -1.0f, 0.0f);

    if(height > width)
    { 
      glTranslatef(0.0f, -0.5f*(height/(float)width)+0.5f, 0.0f);
    } else {
      glTranslatef(0.5f*(width/(float)height)-0.5f, 0.0f, 0.0f);
    }    
  }

  if(projection == PERSPECTIVE)
  {
    glTranslatef(-0.5f,-0.5f,0.0f);
    glTranslatef(0.0f, 0.0f, -display->zoom);
    
/*    glRotatef(60, 1.0f, 0.0f, 0.0f);*/
    /* fuer Drehung */
/*    glRotatef(cam->rx, 1.0f, 0.0f, 0.0f);
    glRotatef(cam->ry, 0.0f, 1.0f, 0.0f);
    glRotatef(cam->rz, 0.0f, 0.0f, 1.0f);
    */
    /* evtl. fuer Verfolgungsmodus */
  /*  drehung = (asin(stars[0].vy/stars[0].vx)/(2*M_PI))*360;
    glRotatef(90, -1.0f, 0.0f, 0.0f);
    glRotatef(90+drehung, 0.0f, 0.0f, 1.0f);
    glTranslatef( -(stars[0].x - 0.5), -(0.5 - stars[0].y), 0.0f );
    glTranslatef( stars[0].vx/500000, stars[0].vy/500000, 0.0f);*/
  }


  /* Galaxienmittelpunkte */
  glBindTexture( GL_TEXTURE_2D, display->texture[IMG_BULGE] );

  if(uni->galaxies[0].exists)
  {
    if(uni->galaxies[0].inLevel) glColor3f(0.0f,1.0f,0.0f);
    drawSkymass(uni->galaxies[0], BULGESIZE);
    glColor3f(1.0f,1.0f,1.0f);
  }

  for(i = 1; i < uni->galaxiesSize; i++)
  {
    if(uni->galaxies[i].exists)
    {
      drawSkymass(uni->galaxies[i], BULGESIZE);
    }
  }
  
  /* Sterne */
  glBindTexture( GL_TEXTURE_2D, display->texture[0] );

  for(i = 0; i < uni->starsSize; i++)
  {
    if(uni->stars[i].exists)
    {
      drawSkymass(uni->stars[i], STARSIZE);
    }
  }

  /* Schwarze Loecher */
  glBindTexture( GL_TEXTURE_2D, display->texture[1] );

  for(i = 0; i < uni->holesSize; i++)
  {
    drawSkymass(uni->holes[i], HOLESIZE*(sqrt(uni->holes[i].mass/HOLEMEDIUMMASS)));
  }

  /* Ziel */  
  glEnable(GL_LIGHT1);
  glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);	
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  
  glLineWidth(1);
glPushMatrix();

  glColor3f(0.0,0.1,0.0);
  glTranslatef( uni->goal.x, uni->goal.y, 0.0f );
  glRotatef(51.0,0.6,0.4,0.0);
  gluQuadricTexture(pObj, 1);
  glBindTexture(GL_TEXTURE_2D, 0);
  gluSphere(pObj, uni->goal.r, 20, 20);
  glTranslatef( -1000000*uni->goal.x, -uni->goal.y, 0.0f );
  glColor3f(1,1,1);

glPopMatrix();

  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  gluDeleteQuadric(pObj);

  glDisable(GL_LIGHT1);
}

void alignPutButtons(GLdisplay *display)
{
  double center, ypos, margin;

  clearButtons(display);
  center = display->width-UNIVERSE_RIGHT+(UNIVERSE_RIGHT/2.0);
   
  /* Simulation starten */
  addButton(display, IMG_START, center, display->height-UNIVERSE_BOTTOM-(START_BUTTON*1.2), START_BUTTON, BUT_startSimulation);

  /* Exit */
  addButton(display, IMG_EXIT, display->width-2*EXIT_BUTTON, UNIVERSE_TOP+2*EXIT_BUTTON, EXIT_BUTTON, BUT_exit);

  ypos = display->height*0.35;
  margin = UNIVERSE_RIGHT*0.3;

  #define TEXTURE(size) ((display->state.holeWeight-(HOLESMALLMASS/2) < (size) && display->state.holeWeight+(HOLESMALLMASS/2) > (size))?IMG_ACTIVE:IMG_BUTTON) 
  addButton(display, TEXTURE(HOLESMALLMASS), center-margin, ypos, SMALL_HOLE, BUT_smallHole);
  addButton(display, TEXTURE(HOLEMEDIUMMASS), center, ypos, MEDIUM_HOLE, BUT_mediumHole);
  addButton(display, TEXTURE(HOLELARGEMASS), center+margin, ypos, LARGE_HOLE, BUT_largeHole);
  #undef TEXTURE
}

void alignSimulButtons(GLdisplay *display)
{
  double center;

  clearButtons(display);
  center = display->width-UNIVERSE_RIGHT+(UNIVERSE_RIGHT/2.0);
   
  /* Simulation stoppen */
  addButton(display, IMG_STOP, center, display->height-UNIVERSE_BOTTOM-(START_BUTTON*1.2), START_BUTTON, BUT_breakSimulation);

  /* Replay */
  addButton(display, IMG_REPLAY, center+REPLAY_BUTTON, display->height-UNIVERSE_BOTTOM-(START_BUTTON*2.1)-REPLAY_BUTTON, REPLAY_BUTTON, BUT_replaySimulation);

  /* Exit */
  addButton(display, IMG_EXIT, display->width-2*EXIT_BUTTON, UNIVERSE_TOP+2*EXIT_BUTTON, EXIT_BUTTON, BUT_exit);
}

void drawBridge(GLdisplay *display, Universe *uni, int projection, int time)
{
  double center;
  int mrx, mry, y, curse, i;
  double mrangle;
  double width, height;

  /* Bildschirm loeschen */
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  
  width = display->width-(UNIVERSE_RIGHT+UNIVERSE_LEFT);
  height = display->height-(UNIVERSE_TOP+UNIVERSE_BOTTOM);

  /* Auf Projektionsmodus umschalten */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0,0,display->width,display->height);

  /* Projektion einstellen */
  /* blending */
  glDisable( GL_BLEND );
  glOrtho(0,display->width,0,display->height,0,128);  

  /* Zurueckschalten und Ansicht zuruecksetzen */
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();

  glScalef(1.0f, -1.0f, 1.0f);
  glTranslatef(0.0f, -display->height, 0.0f);

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
  glPrint( display, TEXTR, TEXTG, TEXTB, 0.0f, TEXTSPACE*(y++), "Task: Navigate the green galaxy into the green target area.");
  for(i = 0; i < uni->galaxiesSize; i++)
  {
    glPrint( display, TEXTR, TEXTG, TEXTB, 0.0f, TEXTSPACE*(y++), "%i. Galaxy: Mass: %.0e kg, Curse: %i°",(i+1),uni->galaxies[0].mass,(int)round((-atan2(uni->galaxies[i].vy,uni->galaxies[i].vx)-0.5*M_PI)*(360/(2*M_PI))));
  }
 /* putImage( IMG_BACKGROUND, 0, 0, display->width, display->height, display );*/
/*  drawRect( 0.0, 0.0, 0.0, UNIVERSE_LEFT, UNIVERSE_TOP, width, height );*/
  putImage( (projection==ORTHOGONAL)?IMG_PANEL_MASS:IMG_PANEL_TIME, display->width-UNIVERSE_RIGHT, 0, UNIVERSE_RIGHT, display->height, display );

  center = display->width-UNIVERSE_RIGHT+(UNIVERSE_RIGHT/2.0);

  if(projection == ORTHOGONAL)
  {
    alignPutButtons(display);
    mrangle = (double)(uni->massreserve)/(double)(MAXSTARTRESERVE);
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
    mrx = - (-display->height*0.11 * sin(mrangle * M_PI/180));
    mry = (-display->height*0.11 * cos(mrangle * M_PI/180));

    glLineWidth(4);

    glColor3f(0,0,0);
    glBegin( GL_LINES );
      glVertex3f( center+mrx, display->height*0.28+mry, 0.0 );
      glVertex3f( center, display->height*0.28, 0.0 );
    glEnd();
    glColor3f(1,1,1);
  }

  drawButtons(display);

  glDisable(GL_POLYGON_SMOOTH);
 
  drawButtons(display);

  displayUniverse(display, uni, projection, width, height);

  /* Versteckten Buffer aktivieren */
  SDL_GL_SwapBuffers();
}

/* Zeichnet die Oberflaeche zum Setzen der Galaxien */
void drawPut(GLdisplay *display, Universe *uni)
{ 
  drawBridge(display, uni, ORTHOGONAL, 0);
}

/* Zeichnet die Simulation */
void drawSimulation( GLdisplay *display, Universe *uni, Kamera *cam, int time )
{
  drawBridge(display, uni, PERSPECTIVE, time);
}

void handleEvents(GLdisplay *display, int part, Universe *uni)
{
  int i, j, run, delete;

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

  while ( SDL_PollEvent( &display->event ) )
  {
    switch( display->event.type )
    {
      case SDL_ACTIVEEVENT:
        /* Fokus verloren (z.B. minimiert) */
        if ( display->event.active.gain == 0 )
        {
          display->isActive = 0;
        }
        else
        {
          display->isActive = 1;
        }  
      case SDL_MOUSEBUTTONDOWN:
        /* Buttons */
        if(part == PUT || part == SIMULATION)
        {
          checkButtons(display);
        }
      
        /* Nur fuer Setzfenster */
        if(part == PUT)
        {        
          /* schwarzes Loch setzen */
          if(display->event.motion.x > UNIVERSE_LEFT && 
             display->event.motion.x < display->width-(UNIVERSE_RIGHT+UNIVERSE_LEFT) && 
             display->event.motion.y > UNIVERSE_TOP && 
             display->event.motion.y < display->height-(UNIVERSE_TOP+UNIVERSE_BOTTOM)
            )
          {
            delete = 0;
                        
            /* Mausposition umrechnen */
            glGetIntegerv(GL_VIEWPORT,viewport);
            glGetDoublev(GL_PROJECTION_MATRIX,projMatrix);
            glGetDoublev(GL_MODELVIEW_MATRIX,modelMatrix);
            glReadPixels(display->event.motion.x, display->event.motion.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &zpos );
            gluUnProject(display->event.motion.x, display->event.motion.y, zpos,
                modelMatrix, projMatrix, viewport,
                &mousex, &mousey, &mousez
            );
            
            mousey = 1.0-mousey;
            
            for(i = 0; i < uni->holesSize; i++)
            {
              if( (pow(mousex-uni->holes[i].x,2)+pow(mousey-uni->holes[i].y,2))
                    <= pow(HOLESIZE*(sqrt(uni->holes[i].mass/HOLEMEDIUMMASS)),2) )
              {
                delete = 1;
                
                /* pruefen ob der ueberhaupt geloescht werden darf */
                if(!uni->holes[i].inLevel)
                {
                  uni->massreserve += uni->holes[i].mass;

                  /* die danach nachruecken lassen */
                  for(j = i; j < (uni->holesSize-1); j++)
                  {
                    uni->holes[j] = uni->holes[j+1];
                  }
                  uni->holesSize--;
                }
              }
            }
            
            if(!delete && uni->massreserve >= display->state.holeWeight)
            {
              newHole.x = mousex;
              newHole.y = mousey;
              newHole.mass = display->state.holeWeight;
              uni->massreserve -= newHole.mass;
              uni->holes[uni->holesSize++] = newHole;
            }
            
          }
          drawPut( display, uni );
        }
        break;
      case SDL_VIDEORESIZE:
        /* Groesse vom Fenster geaendert */
        resizeWindow( display, display->event.resize.w, display->event.resize.h );
        
        if(part == PUT)
        {
          drawPut( display, uni );
        }
        /* Simulation wird sowieso gleich wieder gezeichnet */
        break;
      case SDL_QUIT:
        free(uni->stars);
        exitApp( display );
        break;
      case SDL_KEYDOWN:
        switch(display->event.key.keysym.sym)
        {
          case SDLK_ESCAPE:
            free(uni->stars);
            exitApp( display );
            break;
          case SDLK_SPACE:
            display->state.breakIntro = 1;
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

