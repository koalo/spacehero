#ifndef _UNIVERSE_H_
#define _UNIVERSE_H_

class Universe;
#include <fstream>

#include "GLdisplay.h"

#include "local.h"

#define R_MIN_CENTER 0.02
#define R_MIN 0.0005
#define A 200
#define M 5

#define ORBITS_MAX 130

class skymass
{
	public:
		double x, y, z; /* in 600000 LJ (0.5 = 300000 LJ = Mitte vom Spielfeld) */
		double vx, vy, vz; /* in m/s */
		double fx, fy, fz; /* Kraft in Newton auf den Koerper */
		double mass; /* in Sonnenmassen */
		int inLevel; /* ob das Objekt durch das Level vorgegeben ist */
		int exists;
		int nograv;
	public:
		static void constructGalaxy(skymass *galaxy, skymass **INstars, int *starsSize);
		skymass(const skymass& p);
		skymass() {};
};

class skygoal
{
	public:
		double x, y, z; /* in 600000 LJ (0.5 = 300000 LJ = Mitte vom Spielfeld) */
		double r; /* Radius */  
};

void constructGalaxy(skymass *galaxy, skymass **INstars, int *starsSize);

class Universe
{
	public:
		skygoal goal;
		skymass holes[MAX_LEVEL_HOLES+MAX_PUT_HOLES];
		skymass galaxies[MAX_GALAXIES];
		skymass *stars;
		int holesSize;
		int galaxiesSize;
		int starsSize;
		double massreserve;

	public:
		/* Zeichnet die Oberflaeche zum Setzen der Galaxien */
		void drawPut(GLdisplay &display);
		/* Zeichnet die Simulation */
		void drawSimulation( GLdisplay &display, Kamera *cam, int time ); 
		void move(int time);

		inline void drawSkymass(skymass body, float size)
		{
			glBegin(GL_QUADS);
			glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -size+body.x, -size+body.y, body.z );
			glTexCoord2f( 1.0f, 0.0f ); glVertex3f(  size+body.x, -size+body.y, body.z );
			glTexCoord2f( 1.0f, 1.0f ); glVertex3f(  size+body.x,  size+body.y, body.z );
			glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -size+body.x,  size+body.y, body.z );
			glEnd();
			/* Objekt zeichnen */
			/*
			   glBegin(GL_QUADS);
			   glTexCoord2f( 0.0f, 0.0f ); glVertex3f( body.x, -size+body.y, -size+body.z );
			   glTexCoord2f( 1.0f, 0.0f ); glVertex3f( body.x, -size+body.y, size+body.z );
			   glTexCoord2f( 1.0f, 1.0f ); glVertex3f( body.x,  size+body.y, size+body.z );
			   glTexCoord2f( 0.0f, 1.0f ); glVertex3f( body.x,  size+body.y, -size+body.z );
			   glEnd();

			   glBegin(GL_QUADS);
			   glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -size+body.x, body.y, -size+body.z );
			   glTexCoord2f( 1.0f, 0.0f ); glVertex3f( -size+body.x, body.y, size+body.z );
			   glTexCoord2f( 1.0f, 1.0f ); glVertex3f( size+body.x,  body.y, size+body.z );
			   glTexCoord2f( 0.0f, 1.0f ); glVertex3f( size+body.x,  body.y, -size+body.z );
			   glEnd();*/
		} ;

	private:
		void drawBridge(GLdisplay &display, int projection, int time);
		void alignSimulButtons(GLdisplay &display);
		void alignPutButtons(GLdisplay &display);

	public:
		int levelladen(std::ifstream &level);
		void eventHorizon();
	private:
		inline void applyNewton(skymass* skymass1, skymass* skymass2, int teiler)
		{
			register double AX, AY, a1, a2, r3;

			/* Verbindungsvektor */    
			AX = skymass2->x - skymass1->x;
			AY = skymass2->y - skymass1->y;

			r3 = hypot(AX,AY);
			r3 = r3*r3*r3;

			/* Einheitsvektor in Verbindungsrichtung */
			AX = AX/r3;
			AY = AY/r3;
			/*  AZ = AZ/r3;*/

			/* Beschleunigung */
			a1 = SUNGRAVTIMEWIDTH*teiler*skymass2->mass;
			a2 = SUNGRAVTIMEWIDTH*teiler*skymass1->mass;

			/* Geschwindigkeitsaenderung */
			skymass1->vx += a1*AX;
			skymass1->vy += a1*AY;  
			/*  skymass1->vz += a1*AZ;  */
			skymass2->vx -= a2*AX;
			skymass2->vy -= a2*AY;
			/*  skymass2->vz -= a2*AZ;*/
			/*  ((SUNGRAVTIMEWIDTH/r2)*skmass)*(AX/r)
			    = (SUNGRAVTIMEWIDTH*skymass*AX)/r3*/
		}

};











#endif


