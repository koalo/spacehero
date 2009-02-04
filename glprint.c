#include "glprint.h"

void KillFont( GLdisplay *display )
{
  glDeleteLists( display->fontbase, 96 );
}

void buildFont( GLdisplay *display )
{
  XFontStruct *fontInfo;
  Display *tmpdisplay;

  /* Display oeffnen, Font auswaehlen */
  tmpdisplay = XOpenDisplay( NULL );
  display->fontbase = glGenLists( 96 );
  fontInfo = XLoadQueryFont( tmpdisplay, "-adobe-courier-bold-r-*-*-*-100-*-*-*-*-*-*" ); /* xfontsel -print */

  if ( fontInfo == NULL )
  {
    printf("Schrift nicht verfuegbar.\n");
	}

  glXUseXFont( fontInfo->fid, 32, 96, display->fontbase );
  
  /* Aufraeumen */
  XFreeFont( tmpdisplay, fontInfo );
  XCloseDisplay( tmpdisplay );
}

/* Print our GL text to the screen */
GLvoid glPrint( GLdisplay *display, float red, float green, float blue, float x, float y, const char *format, ... )
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
  glListBase( display->fontbase - 32 );
  
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

