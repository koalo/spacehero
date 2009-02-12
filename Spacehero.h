#ifndef SPACEHERO_H
#define SPACEHERO_H

#include "GLdisplay.h"
#include "Universe.h"

class Spacehero
{
	enum SpaceheroState { 
		spacehero_edit, 
		spacehero_simulate, 
		spacehero_next, 
		spacehero_exit 
	} state;

	bool won;

	GLdisplay &display;
	Universe &universe;

public:

	Spacehero(GLdisplay &d, Universe &u);

	bool play();

	SpaceheroState edit();
	SpaceheroState simulate();
	SpaceheroState handleEvents();


};
#endif
