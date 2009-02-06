#ifndef _GLDISPLAY_H_
#define _GLDISPLAY_H_

class GLdisplay;
class Kamera;
class Button;
#include <iostream>
#include <fstream>

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>
#include <math.h>

#include "BStatus.h"

#include "Universe.h"


class Button
{
	public:
	int texture;
	float x;
	float y;
	float r;
	int active;
	void (*action)(BStatus &state);
};


class Kamera
{
	public:
	float rx;
	float ry;
	float rz;
};

class GLdisplay
{
	public:
	enum Images {
 		IMG_STAR = 0,
		IMG_HOLE,
		IMG_BLACK,
		IMG_GOAL,
		IMG_PANEL_MASS,
		IMG_PANEL_TIME,
		IMG_BUTTON,
		IMG_START,
		IMG_STOP,
		IMG_REPLAY,
		IMG_EXIT,
		IMG_SPACEHERO,
		IMG_BULGE,
		IMG_WIN,
		IMG_LOST,
		IMG_ACTIVE,
		TEXTURES
	} ;
	private:
	SDL_Surface *surface;
	int videoFlags; /* Flags fuer Videoinitialisierung */
	const SDL_VideoInfo *videoInfo; /* Infos ueber Video */
	public:
	int isActive; /* Fenster hat Fokus? */
	SDL_Event event;
	GLuint texture[TEXTURES]; /* Texturspeicher */  
	float zoom;
	int width;
	int height;
	int bpp;
	private:
#define MAX_BUTTONS 10
	Button buttons[MAX_BUTTONS];
	int buttonSize;
	GLuint fontbase;

	public:
	BStatus state;

	public:
	GLdisplay(bool fullscreen=false, int width=600, int height=600, int bpp=16);
	~GLdisplay();

	void startRound(std::ifstream &level);

	void resizeWindow(int width, int height);

	void putImage(Images t, float x, float y, float width, float height);
	void drawRect(float red, float green, float blue, float x, float y, float width, float height);

	void drawSphere(Images t, float x, float y, float r);
	void drawDisk(int t, float x, float y, float r);

	void addButton(Images t, float x, float y, float r, void (*action)(BStatus &state));
	void clearButtons();
	void drawButtons();
	void checkButtons();

	void KillFont();
	void buildFont();
	void glPrint(float red, float green, float blue, float x, float y, const char *format, ... );

	private:

	int LoadGLTextures(GLuint texture[]);



};



#endif
