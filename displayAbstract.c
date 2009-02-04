#include "displayAbstract.h"

int LoadGLTextures(GLuint texture[])
{
  int i, success = 0;
  SDL_Surface *TextureImage[TEXTURES]; 

  glGenTextures( TEXTURES, texture ); /* Texturen vorbereiten */

  /* Textur einladen */
  if( (TextureImage[0] = SDL_LoadBMP( "data/star.bmp" )) &&
      (TextureImage[1] = SDL_LoadBMP( "data/hole.bmp" )) &&
      (TextureImage[2] = SDL_LoadBMP( "data/background.bmp" )) &&
      (TextureImage[3] = SDL_LoadBMP( "data/black.bmp" )) &&
      (TextureImage[4] = SDL_LoadBMP( "data/goal.bmp" ))  &&
      (TextureImage[5] = SDL_LoadBMP( "data/panel_MASS.bmp" ))  &&
      (TextureImage[6] = SDL_LoadBMP( "data/panel_TIME.bmp" ))  &&
      (TextureImage[7] = SDL_LoadBMP( "data/green.bmp" ))  &&
      (TextureImage[8] = SDL_LoadBMP( "data/button_start.bmp" )) &&
      (TextureImage[9] = SDL_LoadBMP( "data/button_stop.bmp" )) &&
      (TextureImage[10] = SDL_LoadBMP( "data/button_replay.bmp" )) &&
      (TextureImage[11] = SDL_LoadBMP( "data/x.bmp" )) &&
      (TextureImage[12] = SDL_LoadBMP( "data/spacehero.bmp" )) &&
      (TextureImage[13] = SDL_LoadBMP( "data/bulge.bmp" )) &&
      (TextureImage[14] = SDL_LoadBMP( "data/accomplished.bmp" )) &&
      (TextureImage[15] = SDL_LoadBMP( "data/timesup.bmp" )) &&
      (TextureImage[16] = SDL_LoadBMP( "data/red.bmp" )) )
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

void initDisplay(GLdisplay *display)
{
  /* Grafik initialisieren */
  if(SDL_Init( SDL_INIT_VIDEO ) < 0)
  {
    fprintf( stderr, "Grafik konnte nicht initialisiert werden: %s\n", SDL_GetError( ) );
    SDL_Quit();
    exit(1);
  }

  /* Grafikparameter ermitteln */
  display->videoInfo = SDL_GetVideoInfo( );

  if(!display->videoInfo)
  {
    fprintf( stderr, "Grafikparameter nicht ermittelbar: %s\n", SDL_GetError( ) );
    SDL_Quit();
    exit(1);
  }

  /* Einstellungen */
  display->videoFlags  = SDL_OPENGL;          /* OpenGL aktivieren */
  display->videoFlags |= SDL_RESIZABLE;       /* Fenster darf veraendert werden */
  display->videoFlags |= SDL_HWPALETTE;       /* Palette in Hardware */
  display->videoFlags |= SDL_GL_DOUBLEBUFFER; /* double buffering */

#ifdef FULLSCREEN
  display->videoFlags |= SDL_FULLSCREEN;
  display->width = display->videoInfo->current_w;
  display->height = display->videoInfo->current_h;
#else
  display->width = SCREEN_WIDTH;
  display->height = SCREEN_HEIGHT;
#endif

  /* Surfaces in Hardware, falls moeglich */
  if (display->videoInfo->hw_available)
  {
    display->videoFlags |= SDL_HWSURFACE;
  }
  else
  {
    display->videoFlags |= SDL_SWSURFACE;
  }

  /* Hardwarebeschleunigung */
  if(display->videoInfo->blit_hw)
  {
    display->videoFlags |= SDL_HWACCEL;
  }

  /* double buffering */
  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

  /* Surface aktivieren */
  display->surface = SDL_SetVideoMode( display->width, display->height, SCREEN_BPP, display->videoFlags );

  if ( !display->surface )
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
  if (!LoadGLTextures(display->texture))
  {
    fprintf( stderr, "Textur konnte nicht geladen werden\n" );   
  }

  /* Font einladen */
  buildFont(display);
}

void resizeWindow(GLdisplay *display, int width, int height)
{
  /* Surface aktivieren */
  display->surface = SDL_SetVideoMode( width, height, SCREEN_BPP, display->videoFlags );

  if ( !display->surface )
  {
    fprintf( stderr,  "Surface konnte nicht aktiviert werden: %s\n", SDL_GetError( ) );
    SDL_Quit();
    exit(1);
  }

  display->width = width;
  display->height = height;
}

void drawRect(float red, float green, float blue, float x, float y, float width, float height)
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

void drawSphere(int texture, float x, float y, float r, GLdisplay *display)
{
  GLUquadricObj *pObj = gluNewQuadric();

  glEnable(GL_LIGHT3);
  
  glTranslatef( x, y, 0.0f );
  gluQuadricTexture(pObj, 1);
  glBindTexture(GL_TEXTURE_2D, display->texture[texture]);
  gluSphere(pObj, r, 30, 30);
  glTranslatef( -x, -y, 0.0f );
  gluDeleteQuadric(pObj);

  glDisable(GL_LIGHT3);
}

void drawDisk(int texture, float x, float y, float r, GLdisplay *display)
{
  GLUquadricObj *pObj = gluNewQuadric();

  glTranslatef( x, y, 0.0f );
  gluQuadricTexture(pObj, 1);
  glBindTexture(GL_TEXTURE_2D, display->texture[texture]);
  gluDisk(pObj, 0, r, 40, 1);
  glTranslatef( -x, -y, 0.0f );

  gluDeleteQuadric(pObj);
}


void putImage(int texture, float x, float y, float width, float height, GLdisplay *display)
{
  /* Textur auswahlen */
  glBindTexture( GL_TEXTURE_2D, display->texture[texture] );

  /* Objekt zeichnen */
  glBegin(GL_QUADS);
    glTexCoord2f( 0.0f, 0.0f ); glVertex3f( x, y, 0.0f );
    glTexCoord2f( 1.0f, 0.0f ); glVertex3f( x+width, y, 0.0f );
    glTexCoord2f( 1.0f, 1.0f ); glVertex3f( x+width, y+height, 0.0f );
    glTexCoord2f( 0.0f, 1.0f ); glVertex3f( x, y+height, 0.0f );
  glEnd();
}

void exitApp(GLdisplay *display)
{
  KillFont(display);
  SDL_Quit();
  exit(0);
}

void clearButtons(GLdisplay *display)
{
  display->buttonSize = 0;
}

void addButton(GLdisplay *display, int texture, float x, float y, float r, void (*action)(BStatus *state))
{
  Button newButton;
  
  newButton.x = x;
  newButton.y = y;
  newButton.r = r;
  newButton.texture = texture;
  newButton.active = 1;
  newButton.action = action;
  display->buttons[display->buttonSize++] = newButton;
}

void drawButtons(GLdisplay *display)
{
  int i;
  
  for(i = 0; i < display->buttonSize; i++)
  {
/*    glEnable( GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    drawDisk(IMG_BULGE,display->buttons[i].x,display->buttons[i].y,display->buttons[i].r*3,display);
    glDisable( GL_BLEND );*/
    drawDisk(display->buttons[i].texture,display->buttons[i].x,display->buttons[i].y,display->buttons[i].r,display);
  }
}

void checkButtons(GLdisplay *display)
{
  int i;
  
  for(i = 0; i < display->buttonSize; i++)
  {
    if( sqrt(pow(display->buttons[i].x-display->event.motion.x,2)+pow(display->buttons[i].y-display->event.motion.y,2)) 
         < display->buttons[i].r
      )
    {
      display->buttons[i].action(&display->state);
    }
  }
}

