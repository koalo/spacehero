/* 
 * This file is part of Spacehero.
 * 
 * Spacehero is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Spacehero is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Spacehero.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "boost/filesystem.hpp"
#include <iostream>
#include <fstream>
using namespace boost::filesystem;

#include <stdlib.h>
#include <vector>

#include "SpaceDisplay.h"
#include "Spacehero.h"
#include "Universe.h"
#include "intro.h"
#include "Illustrator.h"

//int main(int argc, char *argv[])
int main()
{ 
  // Find data Dir
  std::vector<std::string> path;

  path.push_back( "./data/" );
  path.push_back( "~/.spacehero/data/" );
  path.push_back( "/usr/share/games/spacehero/data/" );

  std::string dir;
  for(std::vector<std::string>::iterator l = path.begin(); l!=path.end(); l++) {
    std::cerr << "trying: " << *l << std::endl;
    if ( is_directory(*l) ) {
      dir=*l;
      std::cerr << "found: " << dir << std::endl;
      break;
    }
  }

  if(dir == "") {
    std::cerr << "could not find data dir" << std::endl;
    return 1;
  }

  SpaceDisplay display(dir);

  // Load FileManager
  FileManager fileman;
  fileman.addLevelDir( "./level/" );
  fileman.addLevelDir( "~/.spacehero/level/" );
  fileman.addLevelDir( "/usr/share/games/spacehero/level/" );
    
  Spacehero::SpaceheroState state = Spacehero::spacehero_next;

  bool start = true;

  if(true)
  {
    while(true)
    {
      // Intro und Menu
      /*std::string intr = levels+"level1.txt";
      std::ifstream leveli(intr.c_str());
      if(!leveli) 
      {
	std::cerr << "Level ungueltig" << std::endl;
	exit(0);
      }*/
     
      if(start)
      {	
      try { 
	Universe u = Universe();
	u.galaxies.push_back(Galaxy(0.145,0.145,4.3e11,false,false));
	u.galaxies.back().setZ(0.17);
	u.galaxies.back().setVX(110e3);
	u.galaxies.back().setVY(110e3);
	u.galaxies.push_back(Galaxy(1.17,1.17,20e11,false,false));
	u.galaxies.back().setZ(0.17);
	u.galaxies.back().setVX(-176e3);
	u.galaxies.back().setVY(-176e3);
	u.calcStars();
	u.setStargrav(true);
	u.tinit();
	Spacehero sintro(display,u);
	SDL_Event event;
	while(SDL_PollEvent(&event)) display.getDisplay()->handleEvents(event);
	SDL_ShowCursor(SDL_DISABLE);
	state = sintro.play(SpaceDisplay::IntroView);
      } catch (Error::ParseLevel e) {
	std::cerr << e.msg() << std::endl;
      }
      start = false;
      } else {
	Universe u;
	Spacehero smenu(display,u);
        state = smenu.play(SpaceDisplay::MenuView);
      }

      // Menu auswerten	
      if(state == Spacehero::spacehero_exit)
      {
	// Programm beenden
	break;
      } 
      else if(state == Spacehero::spacehero_chooseLevel)
      {
        fileman.loadLevels();
	fileman.LevelMan(display);
      }
      else if(state == Spacehero::spacehero_emptyEditor)
      {
	// Editor starten
	try { 
	  Universe uni = Universe();
	  Spacehero space(display,uni);
	  space.play(SpaceDisplay::EditorView);
	} catch (Error::ParseLevel e) {
	  std::cerr << e.msg() << std::endl;
	}

      }
      else if(state == Spacehero::spacehero_next)
      {
	bool exit = false;
	fileman.loadLevels();
 
	Level l;

	while(!exit && fileman.hasLevel())
	{
	  l = fileman.nextLevel();
          Universe uni(l);
          Spacehero space(display,uni);
	  exit = (space.play() == Spacehero::spacehero_exit);
	} 
      }
    }
  } else {
  /*  std::ifstream level(argv[1]);
    Level l(level);
    std::cerr << l << std::endl;
    Universe u(l);
    Spacehero s(display,u);
    s.play();*/
  }

  return 0;
}


