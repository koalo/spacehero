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
#include "Illustrator.h"

#include <iostream>
using namespace std;

#include <SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

Illustrator::Illustrator(string path) :
  fontbase(0), font(0), fontheight(0), fontpixels(0), fontspace(0.5), input(""), inputtext(""), doinput(false), fontvanchor(NORTH), fonthanchor(WEST)
{
  int i;
  float fx, fy;
  SDL_Surface *fontImage;

  if ((fontImage = IMG_Load( (path + "font.png").c_str() )))
  {
    glGenTextures( 1, &font );
    glBindTexture( GL_TEXTURE_2D, font );

    /* Filter */
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    /* Textur erstellen */
    glTexImage2D( GL_TEXTURE_2D, 0, 4, fontImage->w, fontImage->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, fontImage->pixels );
    fontpixels = fontImage->w/(float)16;

    //cout << "Bildbreite" << fontImage->w << "Fontsize: " << fontsize << endl;
    SDL_FreeSurface(fontImage);
  } else {
    cerr << "Font nicht vorhanden: " << (path + "font.png") << endl;
    exit(1);
  }

  /* Fontliste erzeugen einladen */
  fontbase  = glGenLists( 256 );
  glBindTexture( GL_TEXTURE_2D, font );
  
  for(i = 0; i < 256; i++)
  {
    /* Berechnung der aktuellen Koordinaten */
    fx = 1 - ((i%16) / 16.0f);
    fy = 1 - ((i/16) / 16.0f);

    glNewList( fontbase + (255-i), GL_COMPILE );
      glBegin( GL_QUADS );
        /* unten links */
        glTexCoord2f( fx - 1/16.0, fy );
        glVertex2f( 0, fontpixels );

        /* unten rechts */
        glTexCoord2f( fx, fy );
        glVertex2f( fontpixels, fontpixels );

        /* oben rechts */
        glTexCoord2f( fx, fy - 1/16.0 );
        glVertex2f( fontpixels, 0 );

        /* oben links */
        glTexCoord2f( fx - 1/16.0, fy - 1/16.0);
        glVertex2i( 0, 0 );
      glEnd( );

      /* wieder zurueck, aber nicht ganz, dadurch überlagern sich die Buchstaben und sind enger */
      glTranslatef( fontpixels*fontspace, 0, 0 );
    glEndList( );
  }
}

Illustrator::~Illustrator()
{
  glDeleteLists(fontbase, 96);
}

void Illustrator::startInput(string text)
{
  input = "";
  inputtext = text;
  doinput = true;
}

void Illustrator::stopInput()
{
  doinput = false;
}

bool Illustrator::doingInput()
{
  return doinput;
}

string Illustrator::getInput()
{
  return input;
}

bool Illustrator::handleInput(SDL_Event& event)
{
  if(!doinput) return false;

  if(event.type == SDL_KEYDOWN)
  {
    if((event.key.keysym.sym >= 'a' && event.key.keysym.sym <= 'z') || (event.key.keysym.sym >= '0' && event.key.keysym.sym <= '9'))
    {
      input += toupper(event.key.keysym.sym);
    } 
    else if(event.key.keysym.sym == SDLK_BACKSPACE && input.length() > 0)
    {
      input.erase(input.end()-1);
    }
    else if(event.key.keysym.sym == SDLK_ESCAPE)
    {
      input = "";
      doinput = false;
      return false;
    }
    else if(event.key.keysym.sym == SDLK_RETURN)
    {
      doinput = false;
    }
  }

  return true;
}

void Illustrator::drawInput()
{ 
  if(doinput)
  {
    string text = inputtext + " " + input;
    if((clock() / 20000)  % 2)
    {
      text = text + "_";
    }
    glPrint(10.0, 10.0, text.c_str());
  }
}

void Illustrator::drawLine(float sx, float sy, float ex, float ey, float width, bool arrow, float hypo)
{
  #define ANGLE 0.35
  #define SQUARE(x) ((x)*(x))
  glLineWidth(width);
  glBindTexture( GL_TEXTURE_2D, 0 );
  glBegin(GL_LINES);
  glVertex2f (sx, sy);
  glVertex2f (ex, ey);
  glEnd();
  if(arrow && SQUARE(cos(ANGLE)*hypo*1.3) < SQUARE(sy-ey)+SQUARE(sx-ex))
  {
    float a = atan2(sy-ey, sx-ex);
    glBegin(GL_TRIANGLES);
    glVertex2f (ex, ey);
    glVertex2f (ex+cos(a+ANGLE)*hypo, ey+sin(a+ANGLE)*hypo);
    glVertex2f (ex+cos(a-ANGLE)*hypo, ey+sin(a-ANGLE)*hypo);
    glEnd();
  }
}

void Illustrator::putImage(float x, float y, float width, float height)
{
  /* Objekt zeichnen */
  glBegin(GL_QUADS);
  glTexCoord2f( 0.0f, 0.0f ); glVertex3f( x, y, 0.0f );
  glTexCoord2f( 1.0f, 0.0f ); glVertex3f( x+width, y, 0.0f );
  glTexCoord2f( 1.0f, 1.0f ); glVertex3f( x+width, y+height, 0.0f );
  glTexCoord2f( 0.0f, 1.0f ); glVertex3f( x, y+height, 0.0f );
  glEnd();
}

void Illustrator::drawRect(float red, float green, float blue, float x, float y, float width, float height)
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

void Illustrator::drawSphere(float x, float y, float r)
{
  GLUquadricObj *pObj = gluNewQuadric();

  glEnable(GL_LIGHT3);

  glTranslatef( x, y, 0.0f );
  gluQuadricTexture(pObj, 1);
  gluSphere(pObj, r, 30, 30);
  glTranslatef( -x, -y, 0.0f );
  gluDeleteQuadric(pObj);

  glDisable(GL_LIGHT3);
}

void Illustrator::drawDisk(float x, float y, float r)
{
  GLUquadricObj *pObj = gluNewQuadric();

  glTranslatef( x, y, 0.0f );
  gluQuadricTexture(pObj, 1);
  gluDisk(pObj, 0, r, 40, 1);
  glTranslatef( -x, -y, 0.0f );

  gluDeleteQuadric(pObj);
}

void Illustrator::setFontheight(float px)
{
  fontheight = px;
}

void Illustrator::setFontalign(VAnchor vanchor, HAnchor hanchor)
{
  fontvanchor = vanchor;
  fonthanchor = hanchor;
}

void Illustrator::glPrint(float x, float y, const char *format, ... )
{
  va_list ap;
  char text[256];

  if ( format == NULL ) return;

  glPushMatrix();

  /* Format parsen */
  va_start( ap, format );
  vsprintf( text, format, ap );
  va_end( ap );

  /* Aussehen und Textur shiften einstellen */
  glBindTexture( GL_TEXTURE_2D, font );
  glMatrixMode(GL_TEXTURE);
  glLoadIdentity();
  glScalef(1.0, 1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  glPushAttrib( GL_LIST_BIT );
  glListBase( fontbase - 32 );

  /* Groesse und Position anpassen */
  switch(fonthanchor)
  {
    case WEST:
      break;
    case CENTER:
      x = x - 0.5*getFontwidth()*strlen(text);
      break;
    case EAST:
      x = x - getFontwidth()*strlen(text);
      break;
  }

  switch(fontvanchor)
  {
    case NORTH:
      break;
    case MIDDLE:
      y = y - 0.5*getFontheight();
      break;
    case SOUTH:
      y = y - getFontheight();
      break;
  }

  glTranslatef(x,y,0.0);
  glScalef(fontheight/fontpixels,fontheight/fontpixels,1.0);
  y += fontheight+3.0f;

  /* richtige Schrift */
  glCallLists( strlen(text), GL_BYTE, text );

  /* zurueckstellen */
  glPopAttrib();
  glDisable( GL_BLEND );
  glPopMatrix();
}
