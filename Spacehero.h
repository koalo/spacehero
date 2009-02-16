#ifndef SPACEHERO_H
#define SPACEHERO_H

#include "SpaceDisplay.h"
#include "Universe.h"
#include "ButtonFlags.h"

class Spacehero
{
  private:
    Spacehero(const Spacehero&);
    Spacehero& operator=(const Spacehero&);
  private:
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

  ButtonFlags bflags;
  Editor editor;

	SpaceDisplay &display;
	Universe &universe;
  Universe *paruni;


  static const double maxframerate = 1.0/50.0;

public:

	Spacehero(SpaceDisplay &d, Universe &u);

	bool play();

	SpaceheroState edit();
	SpaceheroState simulate();
	SpaceheroState handleEvents();


};
#endif
