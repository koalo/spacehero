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

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

#include <boost/filesystem.hpp>
using namespace boost::filesystem;

#include "SpaceDisplay.h"
#include "FileManager.h"
#include "Spacehero.h"

int main(int argc, char *argv[])
{ 
  // Find data Dir
  vector<string> path;

  path.push_back( "./data/" );
  path.push_back( "~/.spacehero/data/" );
  path.push_back( "/usr/share/games/spacehero/data/" );

  string dir;
  for(vector<string>::iterator l = path.begin(); l!=path.end(); l++) {
    cerr << "trying: " << *l << endl;
    if ( is_directory(*l) ) {
      dir=*l;
      cerr << "found: " << dir << endl;
      break;
    }
  }

  if(dir == "") {
    cerr << "could not find data dir" << endl;
    return 1;
  }

  SpaceDisplay display(dir);

  // Load FileManager
  FileManager fileman;
  fileman.addLevelDir( "./level/" );
  string home = getenv("HOME");
  fileman.addLevelDir( home + "/.spacehero/" );
  fileman.addLevelDir( "/usr/share/games/spacehero/level/" );
    
  Spacehero::SpaceheroState state = Spacehero::spacehero_next;

  bool start = true;

  if(argc == 1)
  {
    while(true)
    {
      // Intro und Menu
      /*string intr = levels+"level1.txt";
      ifstream leveli(intr.c_str());
      if(!leveli) 
      {
	cerr << "Level ungueltig" << endl;
	exit(0);
      }*/
     
      if(start)
      {	
      try { 
	Universe u = Universe();
	u.galaxies.push_back(Galaxy(0.145,0.145,4.3e11,false,false));
	u.galaxies.back().z =0.17;
	u.galaxies.back().vx = 110e3;
	u.galaxies.back().vy = 110e3;
	u.galaxies.push_back(Galaxy(1.17,1.17,20e11,false,false));
	u.galaxies.back().z = 0.17;
	u.galaxies.back().vx = -176e3;
	u.galaxies.back().vy = -176e3;
	u.calcStars();
	u.setStargrav(true);
	u.tinit();
	Spacehero sintro(display,u);
	SDL_Event event;
	while(SDL_PollEvent(&event)) display.getDisplay()->handleEvents(event);
	SDL_ShowCursor(SDL_DISABLE);
	state = sintro.play(SpaceDisplay::IntroView);
      } catch (Error::ParseLevel e) {
	cerr << e.msg() << endl;
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
	  cerr << e.msg() << endl;
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
    ifstream levelfile(argv[1]);
    Level lll(levelfile);
    cerr << lll << endl;
    Universe u(lll);
    Spacehero s(display,u);
    s.play(SpaceDisplay::ScreenView);
  }

  return 0;
}


