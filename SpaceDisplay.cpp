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
#include "SpaceDisplay.h"
#include "local.h"
#include "Spacehero.h"

SpaceDisplay::SpaceDisplay(std::string path) : 
#ifdef FULLSCREEN
  display(true)
#else
  display()
#endif
  ,textures(path)
  ,illustrator(path)
  ,buttons(textures, illustrator)
  ,isActive(1)
  ,event()
  ,zoom(0.0)
{
  textures.addTexture("star");
  textures.addTexture("hole");
  textures.addTexture("goal");
  textures.addTexture("galaxy");
  textures.addTexture("panel_MASS");
  textures.addTexture("panel_TIME");
  textures.addTexture("panel_EMPTY");
  textures.addTexture("button_green");
  textures.addTexture("button_start");
  textures.addTexture("button_stop");
  textures.addTexture("button_replay");
  textures.addTexture("button_red");
  textures.addTexture("button_x");
  textures.addTexture("spacehero");
  textures.addTexture("bulge");
  textures.addTexture("accomplished");
  textures.addTexture("timesup");
}

void SpaceDisplay::drawBridge(Universe &uni, BridgeView view, double indicator, double holeWeight)
{
  double center, ypos, margin;
  int mrx, mry, y;
  unsigned int i;
  double mrangle =0, curse;
  double width, height;

  /**************************
   * BILDSCHIRM VORBEREITEN *
   **************************/

  /* Bildschirm loeschen */
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  width = display.getWidth()-(UNIVERSE_RIGHT+UNIVERSE_LEFT);
  height = display.getHeight()-(UNIVERSE_TOP+UNIVERSE_BOTTOM);

  /* Auf Projektionsmodus umschalten */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0,0,display.getWidth(),display.getHeight());
  glOrtho(0,display.getWidth(),0,display.getHeight(),0,128);

  /* Blending aus */
  glDisable(GL_BLEND);
  
  /* Zurueckschalten und Ansicht einstellen */
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();

  glScalef(1.0f, -1.0f, 1.0f);
  glTranslatef(0.0f, -display.getHeight(), 0.0f);



  /**************************
   *        INFOTEXT        *
   **************************/
  float fontsize = 18;
  float fmargin = 3;
  y = 0;
  illustrator.glPrint( fontsize, TEXTR, TEXTG, TEXTB, fmargin, fontsize*(y++)+fmargin, "Task: Navigate the green galaxy into the green target area.");
  for(i = 0; i < uni.galaxies.size(); i++)
  {
    if(uni.galaxies[i].exists)
    {
      curse = atan2(uni.galaxies[i].vx,-uni.galaxies[i].vy); /* Vertauscht und VZ geaendert, dadurch quasi acot2 */
      curse = (curse < 0)?curse+2*M_PI:curse;
      curse = curse*(180/M_PI);
      illustrator.glPrint( fontsize, TEXTR, TEXTG, TEXTB, fmargin, fontsize*(y++)+fmargin, "%i. Galaxy: Mass: %.0e kg, Curse: %i°",(i+1),uni.galaxies[0].mass,(int)round(curse));
    }
  }

  illustrator.glPrint( fontsize, TEXTR, TEXTG, TEXTB, fmargin, fontsize*(y++)+fmargin, "fps: %07.2f",uni.fps());
  illustrator.glPrint( fontsize, TEXTR, TEXTG, TEXTB, fmargin, fontsize*(y++)+fmargin, "elapsed: %2.2f",uni.elapsed());
  illustrator.glPrint( fontsize, TEXTR, TEXTG, TEXTB, fmargin, fontsize*(y++)+fmargin, "won: %d",uni.won());



  /**************************
   *    PANEL EINRICHTEN    *
   **************************/
  if(view == PutView)
  {
    textures.useTexture("panel_MASS");
  }
  else if(view == SimulationView)
  {
    textures.useTexture("panel_TIME");
  }
  else if(view == EditorView)
  {
    textures.useTexture("panel_EMPTY");
  }

  illustrator.putImage(display.getWidth()-UNIVERSE_RIGHT, 0, UNIVERSE_RIGHT, display.getHeight());



  /**************************
   *        BUTTONS         *
   **************************/
  center = display.getWidth()-UNIVERSE_RIGHT+(UNIVERSE_RIGHT/2.0);

  buttons.clearButtons();
  center = display.getWidth()-UNIVERSE_RIGHT+(UNIVERSE_RIGHT/2.0);

  /* Exit */
  buttons.addButton("button_x", display.getWidth()-2*EXIT_BUTTON, UNIVERSE_TOP+2*EXIT_BUTTON, EXIT_BUTTON, ButtonFlags::exit);

  if(view == PutView || view == EditorView)
  {  
    if(view == EditorView)
    {
      buttons.addButton("hole", center, display.getHeight()*0.2, START_BUTTON, ButtonFlags::putHole);
      buttons.addButton("galaxy", center, display.getHeight()*0.4, START_BUTTON, ButtonFlags::putBulge);
      buttons.addButton("goal", center, display.getHeight()*0.6, START_BUTTON, ButtonFlags::putGoal);

      ypos = display.getHeight()*0.75;
    } else {
      ypos = display.getHeight()*0.35;
    }

    /* Simulation starten */
    buttons.addButton("button_start", center, display.getHeight()-UNIVERSE_BOTTOM-(START_BUTTON*1.2), START_BUTTON, ButtonFlags::startSimulation);

    margin = UNIVERSE_RIGHT*0.3;

    #define TEXTURE(size) ((holeWeight-(HOLESMALLMASS/2) < (size) && holeWeight+(HOLESMALLMASS/2) > (size))?"button_red":"button_green")
    buttons.addButton(TEXTURE(HOLESMALLMASS), center-margin, ypos, SMALL_HOLE, ButtonFlags::small);
    buttons.addButton(TEXTURE(HOLEMEDIUMMASS), center, ypos, MEDIUM_HOLE, ButtonFlags::medium);
    buttons.addButton(TEXTURE(HOLELARGEMASS), center+margin, ypos, LARGE_HOLE, ButtonFlags::large);
    #undef TEXTURE
  }

  if(view == SimulationView)
  {
    /* Simulation stoppen */
    buttons.addButton("button_stop", center, display.getHeight()-UNIVERSE_BOTTOM-(START_BUTTON*1.2), START_BUTTON, ButtonFlags::breakSimulation);

    /* Replay */
    buttons.addButton("button_replay", center+REPLAY_BUTTON, display.getHeight()-UNIVERSE_BOTTOM-(START_BUTTON*2.1)-REPLAY_BUTTON, REPLAY_BUTTON, ButtonFlags::replaySimulation);
  }

  buttons.drawButtons();



  /**************************
   *         ZEIGER         *
   **************************/
  glEnable(GL_POLYGON_SMOOTH);

  if(view == PutView || view == SimulationView)
  {
    /* Zeigerposition berechnen */
    mrangle = (indicator < 0)?0:indicator;
    mrangle = mrangle*80-40;
    mrx = - (-display.getHeight()*0.11 * sin(mrangle * M_PI/180));
    mry = (-display.getHeight()*0.11 * cos(mrangle * M_PI/180));

    glLineWidth(4);

    glColor3f(0,0,0);
    glBegin( GL_LINES );
    glVertex3f( center+mrx, display.getHeight()*0.28+mry, 0.0 );
    glVertex3f( center, display.getHeight()*0.28, 0.0 );
    glEnd();
    glColor3f(1,1,1);
  }



  /**************************
   *       UNIVERSUM        *
   **************************/
  displayUniverse(uni, (view == SimulationView)?PERSPECTIVE:ORTHOGONAL, width, height);


  /* Versteckten Buffer aktivieren */
  SDL_GL_SwapBuffers();
}



void SpaceDisplay::displayUniverse( Universe &uni, int projection, int width, int height )
{
  unsigned int i;
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
    zoom = (zoomX > zoomY)?zoomX:zoomY;
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
    glTranslatef(0.0f, 0.0f, -zoom);
    
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
  textures.useTexture("bulge");

  for(i = 0; i < uni.galaxies.size(); i++)
  {
    if(uni.galaxies[i].exists)
    {
      if(uni.galaxies[i].getmaster())
      {
        glColor3f(0.0f,1.0f,0.0f);
      } else {
        glColor3f(1.0f,1.0f,1.0f);
      }
      drawSkymass(uni.galaxies[i]);
    }
  }

  glColor3f(1.0f,1.0f,1.0f);
  
  /* Sterne */
  textures.useTexture("star");

  for(i = 0; i < uni.stars.size(); i++)
  {
    if(uni.stars[i].exists)
    {
      drawSkymass(uni.stars[i]);
    }
  }

  /* Schwarze Loecher */
  textures.useTexture("hole");

  for(i = 0; i < uni.holes.size(); i++)
  {
    drawSkymass(uni.holes[i]);
  }

  /* Ziel */  
  glEnable(GL_LIGHT1);
  glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);	
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  
  glLineWidth(1);

  glPushMatrix();
    glColor3f(0.0,0.1,0.0);
    glTranslatef( uni.goal.x, uni.goal.y, 0.0f );
    glRotatef(51.0,0.6,0.4,0.0);
    gluQuadricTexture(pObj, 1);
    textures.noTexture();
    gluSphere(pObj, uni.goal.radius, 20, 20);
    /*glTranslatef( -1000000*uni.goal.x, -uni.goal.y, 0.0f );*/
    glColor3f(1,1,1);
  glPopMatrix();

  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  gluDeleteQuadric(pObj);

  glDisable(GL_LIGHT1);
}

void SpaceDisplay::showEnd(bool win, ButtonFlags &flags)
{
  unsigned int i;
  Universe uni;
  Editor editor(uni);
  double width;

  if(win)
  {
    textures.useTexture("accomplished");
    width = 629.0;
  } else {
    textures.useTexture("timesup");
    width = 265.0;
  }

  illustrator.putImage( 0.5-((width/102)*0.12)*0.5, 0.5-(0.12*0.5), (width/102)*0.12, 0.12 );
  SDL_GL_SwapBuffers();

  for(i = 0; i < 400; i++)
  {
    handleEvents(SpaceDisplay::SimulationView, flags, editor);
    if(flags.viewFlag(ButtonFlags::replaySimulation) || flags.viewFlag(ButtonFlags::breakSimulation) || flags.viewFlag(ButtonFlags::exit))
    {
      break;
    }
    usleep(10000);
  }
}

void SpaceDisplay::handleEvents(BridgeView view, ButtonFlags &flags, Editor &editor)
{
  GLdouble modelMatrix[16];
  GLdouble projMatrix[16];
  int viewport[4];
  double mousex, mousey, mousez, zpos;
  FileManager saveas;

  while ( SDL_PollEvent( &event ) )
  {
    switch( event.type )
    {
      case SDL_ACTIVEEVENT:
        /* Fokus verloren (z.B. minimiert) */
        if ( event.active.gain == 0 )
        {
          isActive = 0;
        }
        else
        {
          isActive = 1;
        }  
        break;
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
      case SDL_VIDEORESIZE:
        /* Groesse vom Fenster geaendert */
        display.resizeWindow( event.resize.w, event.resize.h );
        break;
      case SDL_QUIT:
          exit(0);
        break;
      case SDL_KEYDOWN:
        switch(event.key.keysym.sym)
        {
          case SDLK_ESCAPE:
            exit(0);
            break;
          case SDLK_e:
            flags.activateFlag((AbstractButtonFlags::Actions)ButtonFlags::startEditor);
            break;
          case SDLK_s:
            std::cout << "Wuerde jetzt gespeichert werden in: " << saveas.getFile() << std::endl;
            break;
          case SDLK_SPACE:
            flags.activateFlag((AbstractButtonFlags::Actions)ButtonFlags::breakIntro);
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
