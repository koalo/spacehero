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
#include "GLdisplay.h"

GLdisplay::GLdisplay(bool fullscreen, int width, int height, int bpp):
  surface(0)
  ,videoFlags(0)
  ,videoInfo(0)
  ,width(width)
  ,height(height)
  ,bpp(bpp)
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
    this->width = videoInfo->current_w;
    this->height = videoInfo->current_h;
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
  surface = SDL_SetVideoMode( this->width, this->height, bpp , videoFlags );

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

  /* Fenstergroesse neu auslesen */
  videoInfo = SDL_GetVideoInfo( );
  resizeWindow(videoInfo->current_w,videoInfo->current_h);
}

GLdisplay::~GLdisplay()
{
  SDL_Quit();
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

int GLdisplay::getHeight()
{
  return height;
}

int GLdisplay::getWidth()
{
  return width;
}

void GLdisplay::initDisplay()
{
  /* Fenstergroesse neu auslesen */
  videoInfo = SDL_GetVideoInfo( );
  resizeWindow(videoInfo->current_w,videoInfo->current_h);
  
  /* Bildschirm loeschen */
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  /* Auf Projektionsmodus umschalten */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0,0,getWidth(),getHeight());
  glOrtho(0,getWidth(),0,getHeight(),0,128);

  /* Blending aus */
  glDisable(GL_BLEND);
  
  /* Zurueckschalten und Ansicht einstellen */
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();

  glScalef(1.0f, -1.0f, 1.0f);
  glTranslatef(0.0f, -getHeight(), 0.0f);
}

void GLdisplay::handleEvents(SDL_Event &event)
{
  switch( event.type )
  {
    case SDL_VIDEORESIZE:
      /* Groesse vom Fenster geaendert */
      resizeWindow( event.resize.w, event.resize.h );
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
	default:
	  break;
      }
      break;
    default:
      break;
  }
}
