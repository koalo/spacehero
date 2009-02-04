#ifndef GLPRINT_H
#define GLPRINT_H
/*
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>*/
#include <string.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include "SDL.h"

#include "displayAbstract.h"

GLvoid KillFont( GLdisplay *display );
GLvoid buildFont( GLdisplay *display );
GLvoid glPrint( GLdisplay *display, float red, float green, float blue, float x, float y, const char *format, ... );
 
#endif
