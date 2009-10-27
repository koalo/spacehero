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

Illustrator::Illustrator(std::string path) :
  fontbase(0), font(0), fontsize(0)
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
    fontsize = fontImage->w/(float)16;

    std::cout << fontsize << std::endl;
    SDL_FreeSurface(fontImage);
  } else {
    std::cerr << "Font nicht vorhanden: " << (path + "font.png") << std::endl;
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
        glTexCoord2f( fx - 1/fontsize, fy );
        glVertex2f( 0, fontsize );

        /* unten rechts */
        glTexCoord2f( fx, fy );
        glVertex2f( fontsize, fontsize );

        /* oben rechts */
        glTexCoord2f( fx, fy - 1/fontsize );
        glVertex2f( fontsize, 0 );

        /* oben links */
        glTexCoord2f( fx - 1/fontsize, fy - 1/fontsize);
        glVertex2i( 0, 0 );
      glEnd( );

      /* wieder zurueck, aber nicht ganz, dadurch überlagern sich die Buchstaben und sind enger */
      glTranslatef( fontsize*0.45, 0, 0 );
    glEndList( );
  }
}

Illustrator::~Illustrator()
{
  glDeleteLists(fontbase, 96);
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

void Illustrator::glPrint(float size, float red, float green, float blue, float x, float y, const char *format, ... )
{
  va_list ap;
  char text[256];

  if ( format == NULL ) return;

  /* Format parsen */
  va_start( ap, format );
  vsprintf( text, format, ap );
  va_end( ap );

  /* Aussehen und Textur shiften einstellen */
  glBindTexture( GL_TEXTURE_2D, font );
  glPushMatrix();
  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  //  glEnable(GL_ALPHA_TEST);
//  glAlphaFunc(GL_GREATER, 0.1);
  glPushAttrib( GL_LIST_BIT );
  glListBase( fontbase - 32 );

  /* Groesse und Position anpassen */
  glTranslatef(x,y,0.0);
  glScalef((size/fontsize),(size/fontsize),1.0);
  y += size+3.0f;

  /* richtige Schrift */
  glColor3f(red,green,blue);
  glCallLists( strlen(text), GL_BYTE, text );

  /* zurueckstellen */
  glColor3f(1,1,1);
  glPopAttrib();
  glDisable( GL_BLEND );
  glPopMatrix();
}
