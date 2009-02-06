#include "GLdisplay.h"
#include "handleEvents.h"
#include "Universe.h"

#include "display.h"

void GLdisplay::startRound(std::ifstream &level)
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
  uni.levelladen(level);
  state.mediumHole(this->state);

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

GLdisplay::GLdisplay(bool fullscreen, int width, int height, int bpp):
	width(width), height(height),bpp(bpp)
{
  /* Grafik initialisieren */
  if(SDL_Init( SDL_INIT_VIDEO ) < 0)
  {
    fprintf( stderr, "Grafik konnte nicht initialisiert werden: %s\n", SDL_GetError( ) );
    SDL_Quit();
    exit(1);
  }

  /* Grafikparameter ermitteln */
  videoInfo = SDL_GetVideoInfo( );

  if(!videoInfo)
  {
    fprintf( stderr, "Grafikparameter nicht ermittelbar: %s\n", SDL_GetError( ) );
    SDL_Quit();
    exit(1);
  }

  /* Einstellungen */
  videoFlags  = SDL_OPENGL;          /* OpenGL aktivieren */
  videoFlags |= SDL_RESIZABLE;       /* Fenster darf veraendert werden */
  videoFlags |= SDL_HWPALETTE;       /* Palette in Hardware */
  videoFlags |= SDL_GL_DOUBLEBUFFER; /* double buffering */

  if(fullscreen) {
	  videoFlags |= SDL_FULLSCREEN;
	  width = videoInfo->current_w;
	  height = videoInfo->current_h;
  } else {
  }

  /* Surfaces in Hardware, falls moeglich */
  if (videoInfo->hw_available)
  {
    videoFlags |= SDL_HWSURFACE;
  }
  else
  {
    videoFlags |= SDL_SWSURFACE;
  }

  /* Hardwarebeschleunigung */
  if(videoInfo->blit_hw)
  {
    videoFlags |= SDL_HWACCEL;
  }

  /* double buffering */
  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

  /* Surface aktivieren */
  surface = SDL_SetVideoMode( width, height, bpp , videoFlags );

  if ( !surface )
  {
    fprintf( stderr,  "Surface konnte nicht aktiviert werden: %s\n", SDL_GetError( ) );
    SDL_Quit();
    exit(1);
  }

  /* Titel setzen */
  SDL_WM_SetCaption ("SpaceHero", "SpaceHero");

  glEnable( GL_TEXTURE_2D );                           /* texture mapping */
  glShadeModel( GL_SMOOTH );                           /* smooth shading */
  glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );              /* Hintergrundfarbe */
  glClearDepth( 1.0f );                                /* Tiefenbuffer */
  glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST ); /* Perspektivenberechnung */
  
  /* Textur laden */
  if (!LoadGLTextures(texture))
  {
    fprintf( stderr, "Textur konnte nicht geladen werden\n" );
  }

  /* Font einladen */
  buildFont();
}

void GLdisplay::resizeWindow(int width, int height)
{
  /* Surface aktivieren */
  surface = SDL_SetVideoMode( width, height, bpp, videoFlags );

  if ( !surface )
  {
    fprintf( stderr,  "Surface konnte nicht aktiviert werden: %s\n", SDL_GetError( ) );
    SDL_Quit();
    exit(1);
  }

  this->width = width;
  this->height = height;
}

void GLdisplay::putImage(Images t, float x, float y, float width, float height)
{
  /* Textur auswahlen */
  glBindTexture( GL_TEXTURE_2D, texture[t] );

  /* Objekt zeichnen */
  glBegin(GL_QUADS);
    glTexCoord2f( 0.0f, 0.0f ); glVertex3f( x, y, 0.0f );
    glTexCoord2f( 1.0f, 0.0f ); glVertex3f( x+width, y, 0.0f );
    glTexCoord2f( 1.0f, 1.0f ); glVertex3f( x+width, y+height, 0.0f );
    glTexCoord2f( 0.0f, 1.0f ); glVertex3f( x, y+height, 0.0f );
  glEnd();
}

void GLdisplay::drawRect(float red, float green, float blue, float x, float y, float width, float height)
{
  glBindTexture( GL_TEXTURE_2D, 0 );
  glColor3f(red,green,blue);

  /* Objekt zeichnen */
  glBegin(GL_QUADS);
    glVertex3f( x, y, 0.0f );
    glVertex3f( x+width, y, 0.0f );
    glVertex3f( x+width, y+height, 0.0f );
    glVertex3f( x, y+height, 0.0f );
  glEnd();

  glColor3f(1,1,1);
}

void GLdisplay::drawSphere(Images t, float x, float y, float r)
{
  GLUquadricObj *pObj = gluNewQuadric();

  glEnable(GL_LIGHT3);
  
  glTranslatef( x, y, 0.0f );
  gluQuadricTexture(pObj, 1);
  glBindTexture(GL_TEXTURE_2D, texture[t]);
  gluSphere(pObj, r, 30, 30);
  glTranslatef( -x, -y, 0.0f );
  gluDeleteQuadric(pObj);

  glDisable(GL_LIGHT3);
}

void GLdisplay::drawDisk(int t, float x, float y, float r)
{
  GLUquadricObj *pObj = gluNewQuadric();

  glTranslatef( x, y, 0.0f );
  gluQuadricTexture(pObj, 1);
  glBindTexture(GL_TEXTURE_2D, texture[t]);
  gluDisk(pObj, 0, r, 40, 1);
  glTranslatef( -x, -y, 0.0f );

  gluDeleteQuadric(pObj);
}

GLdisplay::~GLdisplay()
{
  KillFont();
  SDL_Quit();
  exit(0);
}

void GLdisplay::clearButtons()
{
  buttonSize = 0;
}

void GLdisplay::addButton(Images t, float x, float y, float r, void (*action)(BStatus &state))
{
  Button newButton;
  
  newButton.x = x;
  newButton.y = y;
  newButton.r = r;
  newButton.texture = t;
  newButton.active = 1;
  newButton.action = action;
  buttons[buttonSize++] = newButton;
}

void GLdisplay::drawButtons()
{
  int i;
  
  for(i = 0; i < buttonSize; i++)
  {
/*    glEnable( GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    drawDisk(IMG_BULGE,display->buttons[i].x,display->buttons[i].y,display->buttons[i].r*3,display);
    glDisable( GL_BLEND );*/
    drawDisk(buttons[i].texture,buttons[i].x,buttons[i].y,buttons[i].r);
  }
}

void GLdisplay::checkButtons()
{
  int i;
  
  for(i = 0; i < buttonSize; i++)
  {
    if( hypot(buttons[i].x-event.motion.x,buttons[i].y-event.motion.y) < buttons[i].r)
    {
      buttons[i].action(state);
    }
  }
}

int GLdisplay::LoadGLTextures(GLuint texture[])
{
  int i, success = 0;
  SDL_Surface *TextureImage[TEXTURES]; 

  glGenTextures( TEXTURES, texture ); /* Texturen vorbereiten */

  /* Textur einladen */
  if( (TextureImage[0] = SDL_LoadBMP( "data/star.bmp" )) &&
      (TextureImage[1] = SDL_LoadBMP( "data/hole.bmp" )) &&
      (TextureImage[2] = SDL_LoadBMP( "data/black.bmp" )) &&
      (TextureImage[3] = SDL_LoadBMP( "data/goal.bmp" ))  &&
      (TextureImage[4] = SDL_LoadBMP( "data/panel_MASS.bmp" ))  &&
      (TextureImage[5] = SDL_LoadBMP( "data/panel_TIME.bmp" ))  &&
      (TextureImage[6] = SDL_LoadBMP( "data/green.bmp" ))  &&
      (TextureImage[7] = SDL_LoadBMP( "data/button_start.bmp" )) &&
      (TextureImage[8] = SDL_LoadBMP( "data/button_stop.bmp" )) &&
      (TextureImage[9] = SDL_LoadBMP( "data/button_replay.bmp" )) &&
      (TextureImage[10] = SDL_LoadBMP( "data/x.bmp" )) &&
      (TextureImage[11] = SDL_LoadBMP( "data/spacehero.bmp" )) &&
      (TextureImage[12] = SDL_LoadBMP( "data/bulge.bmp" )) &&
      (TextureImage[13] = SDL_LoadBMP( "data/accomplished.bmp" )) &&
      (TextureImage[14] = SDL_LoadBMP( "data/timesup.bmp" )) &&
      (TextureImage[15] = SDL_LoadBMP( "data/red.bmp" )) )
  {
    success = 1;

   
    for (i=0; i < TEXTURES; i++)
    {
      glBindTexture( GL_TEXTURE_2D, texture[i] ); /* Textur einstellen */

      /* Filter */
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

      /* Textur erstellen */
      glTexImage2D( GL_TEXTURE_2D, 0, 3, TextureImage[i]->w, TextureImage[i]->h, 0, GL_BGR, GL_UNSIGNED_BYTE, TextureImage[i]->pixels );
    }
  }

  /* Speicherplatz freigeben */
  for(i=0; i < TEXTURES; i++)
  {
    if (TextureImage[i])
    {
      SDL_FreeSurface(TextureImage[i]);
    }
  }

  return success;
}

void GLdisplay::KillFont( )
{
  glDeleteLists( fontbase, 96 );
}


#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>

void GLdisplay::buildFont()
{
  XFontStruct *fontInfo;
  Display *tmpdisplay;

  /* Display oeffnen, Font auswaehlen */
  tmpdisplay = XOpenDisplay( NULL );
  fontbase = glGenLists( 96 );
  fontInfo = XLoadQueryFont( tmpdisplay, "-adobe-courier-bold-r-*-*-*-100-*-*-*-*-*-*" ); /* xfontsel -print */

  if ( fontInfo == NULL )
  {
    printf("Schrift nicht verfuegbar.\n");
	}

  glXUseXFont( fontInfo->fid, 32, 96, fontbase );
  
  /* Aufraeumen */
  XFreeFont( tmpdisplay, fontInfo );
  XCloseDisplay( tmpdisplay );
}

/* Print our GL text to the screen */
GLvoid GLdisplay::glPrint(float red, float green, float blue, float x, float y, const char *format, ... )
{
  va_list ap;
  char text[256];

  if ( format == NULL ) return;

  /* Format parsen */
  va_start( ap, format );
  vsprintf( text, format, ap );
  va_end( ap );

  /* Aussehen und Textur shiften einstellen */
  glBindTexture( GL_TEXTURE_2D, 0 );
  glDisable(GL_BLEND);
  glPushAttrib( GL_LIST_BIT );
  glListBase( fontbase - 32 );
  
  /* Position anpassen */
  y += 12.0f;
  x += 3.0f;
  
  /* Schatten */
  glColor3f(0.3,0.3,0.3);
  glRasterPos2f(x+0.001,y+0.002);
  glCallLists( strlen( text ), GL_UNSIGNED_BYTE, text );

  /* richtige Schrift */
  glColor3f(red,green,blue);
  glRasterPos2f( x, y );
  glCallLists( strlen( text ), GL_UNSIGNED_BYTE, text );

  glPopAttrib();

  glColor3f(1,1,1);
}

