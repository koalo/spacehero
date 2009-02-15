#include "Illustrator.h"

Illustrator::Illustrator() :
  fontbase(0)
{
  /* Font einladen */
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

Illustrator::~Illustrator()
{
  glDeleteLists(fontbase, 96);
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

void Illustrator::glPrint(float red, float green, float blue, float x, float y, const char *format, ... )
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
