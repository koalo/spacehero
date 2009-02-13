#ifndef SPACEHERO_H
#define SPACEHERO_H

#include "SpaceDisplay.h"
#include "Universe.h"

class Spacehero
{
  // Zustaende des Spielautomaten
  //
  // Endzustaende: next und exit
	enum SpaceheroState { 
		spacehero_edit, 
    spacehero_startsimu,
		spacehero_simulate, 
		spacehero_stopsimu, 
		spacehero_next, 
		spacehero_exit 
	} state;

	bool won;

	SpaceDisplay &display;
	Universe &universe;
  Universe *paruni;

  static const double minframerate = 1.0/20.0;
  static const double maxframerate = 1.0/100.0;

public:

	Spacehero(SpaceDisplay &d, Universe &u);

	bool play();

	SpaceheroState edit();
	SpaceheroState simulate();
	SpaceheroState handleEvents();


};
#endif
