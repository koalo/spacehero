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
  width = display.getWidth()-(UNIVERSE_RIGHT+UNIVERSE_LEFT);
  height = display.getHeight()-(UNIVERSE_TOP+UNIVERSE_BOTTOM);

  display.cleanDisplay();
  display.OrthoMode();
  
  /**************************
   *        INFOTEXT        *
   **************************/
  float fontsize = 20;
  float fmargin = 3;
  y = 0;
  #define Y (fontsize*(y++)+fmargin)
  std::string help = "Task: Navigate the green galaxy";
  bool foundone = false;
  for(unsigned int i = 0; i < uni.galaxies.size(); i++)
  {
    if(uni.galaxies.at(i).getmaster())
    {
      if(!foundone)
      {
	foundone = true;
      } else {
        help += "s";
	break;
      }
    }
  }
  help += " into the green target area.";
  illustrator.glPrint( fontsize, TEXTR, TEXTG, TEXTB, fmargin, Y, help.c_str());
  for(i = 0; i < uni.galaxies.size(); i++)
  {
    if(uni.galaxies[i].exists)
    {
      curse = atan2(uni.galaxies[i].vx,-uni.galaxies[i].vy); /* Vertauscht und VZ geaendert, dadurch quasi acot2 */
      curse = (curse < 0)?curse+2*M_PI:curse;
      curse = curse*(180/M_PI);
      illustrator.glPrint( fontsize, TEXTR, TEXTG, TEXTB, fmargin, Y, "%i. Galaxy: Mass: %.0e kg, Curse: %i°",(i+1),uni.galaxies[0].mass,(int)round(curse));
    }
  }

  if(view == SimulationView)
  { 
    illustrator.glPrint( fontsize, TEXTR, TEXTG, TEXTB, fmargin, Y, "fps: %02.2f",uni.fps());
  }
 // illustrator.glPrint( fontsize, TEXTR, TEXTG, TEXTB, fmargin, Y, "elapsed: %2.2f",uni.elapsed());
 // illustrator.glPrint( fontsize, TEXTR, TEXTG, TEXTB, fmargin, Y, "won: %d",uni.won());
  illustrator.glPrint( fontsize*2, TEXTR, TEXTG, TEXTB, fmargin, display.getHeight()-fontsize*2.3, uni.getName().c_str());
  #undef Y


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

      /* Skip */
      buttons.addButton("button_green", center+REPLAY_BUTTON, display.getHeight()-UNIVERSE_BOTTOM-(START_BUTTON*2.1)-REPLAY_BUTTON, REPLAY_BUTTON, ButtonFlags::skipLevel);
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

    glColor3f(0,0,0);
    illustrator.drawLine(center+mrx, display.getHeight()*0.28+mry, center, display.getHeight()*0.28, 4);
    glColor3f(1,1,1);
  }


   /**************************
   *       UNIVERSUM        *
   **************************/ 
  displayUniverse(uni, width, height, false, true, (view == EditorView || view == PutView));     

  /* Versteckten Buffer aktivieren */
//  SDL_GL_SwapBuffers();
}


void SpaceDisplay::displayUniverse( Universe &uni, int width, int height, bool eye, bool pleft, bool arrows )
{
  if(eye && pleft)
  {
    displayUniverse(uni, width, height, true, false);
  }

  unsigned int i;
  //double eyet = 0;
  GLfloat ratio;
  float zoomX, zoomY, viewrad;
  GLUquadricObj *pObj = gluNewQuadric();
  GLfloat LightPosition[]= { 0.0f, 0.0f, 2.0f, 1.0f };
  GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };	

  /* Universum zeichnen */
  /* blending */
  glBlendFunc( GL_SRC_ALPHA, GL_ONE );
  glEnable( GL_BLEND );
  /*glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   
  glEnable(GL_ALPHA_TEST);
  glAlphaFunc(GL_LESS, 0.1);   
      */
  /* Auf Projektionsmodus umschalten */
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  glViewport(UNIVERSE_LEFT,UNIVERSE_TOP,width,height);

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

  glTranslatef(-0.5f,-0.5f,0.0f);

  drawStars(true,eye,pleft,uni);

  glPushMatrix();

  gluLookAt(0, 0, zoom,
             0,
             0,
             0,
             0,1,0);
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

  glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);

  glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );

/*  glDisable(GL_BLEND);*/

  /* Galaxienmittelpunkte */
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

      textures.useTexture("bulge");
      drawSkymass(uni.galaxies[i]);

      if(arrows)
      {
	glColor4f(0.4,0.4,0,1);
#define ARROWFAC 0.5e-6
	getIllustrator()->drawLine(uni.galaxies.at(i).getX(),uni.galaxies.at(i).getY(),uni.galaxies.at(i).getX()+uni.galaxies.at(i).getVX()*ARROWFAC,uni.galaxies.at(i).getY()+uni.galaxies.at(i).getVY()*ARROWFAC,2,true,0.06);
        glColor4f(1,1,1,1);
      }
    }
  }

  glColor3f(1.0f,1.0f,1.0f);
  
  glBlendFunc( GL_SRC_ALPHA, GL_ONE );
  /* Schwarze Loecher */
  textures.useTexture("hole");

  for(i = 0; i < uni.holes.size(); i++)
  {
    drawSkymass(uni.holes[i]);
  }
  
  /* Ziel */  
  glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
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
    glColor3f(1,1,1);
  glPopMatrix();

  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  gluDeleteQuadric(pObj);

  glDisable(GL_LIGHT1);

  glPopMatrix(); 
  drawStars(false,eye,pleft,uni);
  glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
  if(eye)
  {
    if(pleft)
    { 
      glAccum(GL_LOAD,1.0);
      glDrawBuffer(GL_BACK);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    } else {
      glAccum(GL_ACCUM,1.0);
      glAccum(GL_RETURN,1.0);
    }
  }
}

void SpaceDisplay::drawStars(bool behind, bool eye, bool pleft, Universe &uni)
{
  unsigned int i;
  double eyet;
  glPushMatrix();

  /* Sterne */
  if(eye)
  {
    if(pleft)
    {
      glColorMask(GL_TRUE,GL_FALSE,GL_FALSE,GL_TRUE);
      eyet = -0.05;
    } else {
      glColorMask(GL_FALSE,GL_FALSE,GL_TRUE,GL_TRUE);
      eyet = 0.05;
    }
  }

  gluLookAt(eyet, 0, zoom,
             0,
             0,
             0,
             0,1,0);

  textures.useTexture("star");
  glPointSize(100.0);

  for(i = 0; i < uni.stars.size(); i++)
  {
    if(uni.stars[i].exists && ((behind && uni.stars[i].z < 0) || (!behind && uni.stars[i].z > 0)))
    {
      drawSkymass(uni.stars[i]);
    }
  }

  glPopMatrix();
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
  } 
  else
  {
    textures.useTexture("timesup");
    width = 265.0;
  }
  
  gluLookAt(0, 0, zoom,
             0,
             0,
             0,
             0,1,0);
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

void SpaceDisplay::showMenu(double time)
{
  double scale, herotime, buttontime;

  textures.useTexture("spacehero");

  display.PerspectiveMode(0,0,display.getWidth(),display.getHeight(),VIEWANGLE);
    
  scale = 0.9;
  herotime = 1.0-exp(-(time*100)*0.005);
  herotime = (herotime > 0)?herotime:0;
  herotime = (herotime < scale)?herotime:scale;
  illustrator.putImage( 0.5-scale*herotime/2, 0.5-(scale*(186.0/634.0)*herotime)/2.0, scale*herotime, scale*(186.0/634.0)*herotime );

  buttons.clearButtons();

  display.OrthoMode();

  buttontime = 1.0-exp(-((time-3)*100)*0.009);
  buttontime = (buttontime > 0)?buttontime:0;
  buttontime = (buttontime < scale)?buttontime:scale;
  buttons.addButton("button_x", display.getWidth()*0.8, display.getHeight()*0.8, display.getWidth()*0.08*buttontime, ButtonFlags::exit);
  buttons.addButton("button_start", display.getWidth()*0.2, display.getHeight()*0.2, display.getWidth()*0.08*buttontime, ButtonFlags::startGame);
  buttons.addButton("button_green", display.getWidth()*0.15, display.getHeight()*0.75, display.getWidth()*0.08*buttontime, ButtonFlags::chooseLevel);
  buttons.addButton("button_green", display.getWidth()*0.7, display.getHeight()*0.25, display.getWidth()*0.08*buttontime, ButtonFlags::startEditor);

  buttons.drawButtons();
}

void SpaceDisplay::handleEvents(BridgeView view, ButtonFlags &flags, Editor &editor)
{
  GLdouble modelMatrix[16];
  GLdouble projMatrix[16];
  int viewport[4];
  double mousex, mousey, mousez, zpos;

  while ( SDL_PollEvent( &event ) )
  {
    display.handleEvents(event);

    /* Nur fuer Setzfenster */
    if((view == SpaceDisplay::PutView || view == SpaceDisplay::EditorView) && (event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN))
    {        
      /* Mausposition umrechnen - dazu Orthogonale Ansicht vorgaukeln */ 
      double width = display.getWidth()-(UNIVERSE_RIGHT+UNIVERSE_LEFT);
      double height = display.getHeight()-(UNIVERSE_TOP+UNIVERSE_BOTTOM);
      glMatrixMode( GL_PROJECTION );
      glLoadIdentity();
      gluOrtho2D(0,width,0,height);
      glMatrixMode( GL_MODELVIEW );
      glLoadIdentity();

      if(height > width)
      { 
	glScalef(width, width, 1.0);
      } else {
	glScalef(height, height, 1.0);
      }

      glScalef(1.0f, -1.0f, 1.0f);
      glTranslatef(0.0f, -1.0f, 0.0f);

      if(height > width)
      { 
	glTranslatef(0.0f, -0.5f*(height/(float)width)+0.5f, 0.0f);
      } else {
	glTranslatef(0.5f*(width/(float)height)-0.5f, 0.0f, 0.0f);
      }

      glGetIntegerv(GL_VIEWPORT,viewport);
      glGetDoublev(GL_PROJECTION_MATRIX,projMatrix);
      glGetDoublev(GL_MODELVIEW_MATRIX,modelMatrix);
      glReadPixels(event.motion.x, event.motion.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &zpos );
      gluUnProject(event.motion.x, event.motion.y, zpos,
	  modelMatrix, projMatrix, viewport,
	  &mousex, &mousey, &mousez
      );
      
      mousey = 1.0-mousey;

      bool onSpace = (event.motion.x > UNIVERSE_LEFT && 
       event.motion.x < display.getWidth()-(UNIVERSE_RIGHT+UNIVERSE_LEFT) && 
       event.motion.y > UNIVERSE_TOP && 
       event.motion.y < display.getHeight()-(UNIVERSE_TOP+UNIVERSE_BOTTOM)
      );

      editor.check(mousex,mousey,event.motion.x,event.motion.y,(event.type == SDL_MOUSEBUTTONDOWN),onSpace);
    }

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
      case SDL_MOUSEMOTION:
        if(view == SpaceDisplay::IntroView) SDL_ShowCursor(SDL_ENABLE);
	break;
      case SDL_MOUSEBUTTONDOWN:
        /* Buttons */
        if(!editor.settingGalaxy())
	{
	  if(view == SpaceDisplay::PutView || view == SpaceDisplay::IntroView || view == SpaceDisplay::MenuView || view == SpaceDisplay::SimulationView || view == SpaceDisplay::EditorView)
          {
            buttons.checkButtons(flags,event.motion.x,event.motion.y);
          }

          if(view == SpaceDisplay::IntroView)
	  {
	    flags.activateFlag((AbstractButtonFlags::Actions)ButtonFlags::breakIntro);
	  }
	}
        break;
      case SDL_KEYDOWN:
        switch(event.key.keysym.sym)
        {
          case SDLK_e:
            flags.activateFlag((AbstractButtonFlags::Actions)ButtonFlags::startEditor);
            break;
          case SDLK_s:
            flags.activateFlag((AbstractButtonFlags::Actions)ButtonFlags::saveLevel);
            break;
          case SDLK_SPACE:
            flags.activateFlag((AbstractButtonFlags::Actions)ButtonFlags::breakIntro);
            if(view == SpaceDisplay::IntroView)
	    {
	      flags.activateFlag((AbstractButtonFlags::Actions)ButtonFlags::breakIntro);
	    }
	    break;
	  case SDLK_ESCAPE:
            if(view == SpaceDisplay::IntroView)
	    {
	      flags.activateFlag((AbstractButtonFlags::Actions)ButtonFlags::breakIntro);
	    }
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
