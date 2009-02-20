/* 
 * This file is part of Spacehero.
 * 
 * Spacehero is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Foobar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Spacehero.  If not, see <http://www.gnu.org/licenses/>.
 */
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
		spacehero_starteditor,
		spacehero_stopeditor,
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
