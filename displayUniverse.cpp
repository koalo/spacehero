#include "displayUniverse.h"
#include "display.h"

void displayUniverse( GLdisplay &display, Universe &uni, int projection, int width, int height )
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
    display.zoom = (zoomX > zoomY)?zoomX:zoomY;
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
    glTranslatef(0.0f, 0.0f, -display.zoom);
    
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
  glBindTexture( GL_TEXTURE_2D, display.texture[GLdisplay::IMG_BULGE] );

  if(uni.galaxies[0].exists)
  {
    if(uni.galaxies[0].inLevel) glColor3f(0.0f,1.0f,0.0f);
    uni.drawSkymass(uni.galaxies[0], BULGESIZE);
    glColor3f(1.0f,1.0f,1.0f);
  }

  for(i = 1; i < uni.galaxiesSize; i++)
  {
    if(uni.galaxies[i].exists)
    {
      uni.drawSkymass(uni.galaxies[i], BULGESIZE);
    }
  }
  
  /* Sterne */
  glBindTexture( GL_TEXTURE_2D, display.texture[0] );

  for(i = 0; i < uni.starsSize; i++)
  {
    if(uni.stars[i].exists)
    {
      uni.drawSkymass(uni.stars[i], STARSIZE);
    }
  }

  /* Schwarze Loecher */
  glBindTexture( GL_TEXTURE_2D, display.texture[1] );

  for(i = 0; i < uni.holesSize; i++)
  {
    uni.drawSkymass(uni.holes[i], HOLESIZE*(sqrt(uni.holes[i].mass/HOLEMEDIUMMASS)));
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
  glBindTexture(GL_TEXTURE_2D, 0);
  gluSphere(pObj, uni.goal.r, 20, 20);
  glTranslatef( -1000000*uni.goal.x, -uni.goal.y, 0.0f );
  glColor3f(1,1,1);

glPopMatrix();

  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  gluDeleteQuadric(pObj);

  glDisable(GL_LIGHT1);
}
