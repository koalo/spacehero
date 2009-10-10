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

int main(int argc, char *argv[])
{ 
  std::vector<std::string> path;

  path.push_back( "./" );
  path.push_back( "~/.spacehero/" );
  path.push_back( "/usr/share/games/spacehero/" );

  std::string dir;
  for(std::vector<std::string>::iterator l = path.begin(); l!=path.end(); l++) {
    std::cerr << "trying: " << *l << std::endl;
    if ( is_directory(*l+"level/") && is_directory(*l+"data/") ) {
      dir=*l;
      std::cerr << "found: " << dir << std::endl;
      break;
    }
  }
  if(dir == "") {
    std::cerr << "could not find data/level dir" << std::endl;
    return 1;
  }


  SpaceDisplay display(dir+"data/");
  std::string levels = dir+"level/";
  Spacehero::SpaceheroState state;
  std::cerr << "argc: " << argc << std::endl;

  if(argc<=1) {
    std::cerr << "leveldir: "<< levels << std::endl;
    if (is_directory(levels) ) {
      std::cerr << "level dir found" << std::endl;

      while(true)
      {
	// Intro und Menu
	std::string intr = levels+"level1.txt";
	std::ifstream leveli(intr.c_str());
	if(!leveli) 
	{
	  std::cerr << "Level ungueltig" << std::endl;
	  exit(0);
	}
	
	try { 
	  Level li(leveli); 
	  Universe ui(li);
	  Spacehero si(display,ui);
	  state = si.play(SpaceDisplay::IntroView);
	} catch (Error::ParseLevel e) {
	  std::cerr << e.msg() << std::endl;
	}

        // Menu auswerten	
	if(state == Spacehero::spacehero_exit)
	{
	  // Programm beenden
	  break;
	} 
	else if(state == Spacehero::spacehero_emptyEditor)
	{
          // Editor starten
          try { 
	    std::ifstream level(intr.c_str());
	    Level l(level); 
	    Universe u(l);
	    Spacehero s(display,u);
	    s.play(SpaceDisplay::EditorView);
	   } catch (Error::ParseLevel e) {
	    std::cerr << e.msg() << std::endl;
	   }

	}
	else if(state == Spacehero::spacehero_next)
        {
	  // Alle Level durchgehen
	  for (directory_iterator itr(levels); itr != directory_iterator(); ++itr)
	  {
	    std::cerr << "trying to load level: " << itr->path() << std::endl;
	    std::ifstream level(itr->path().string().c_str());
	    std::ofstream levelwrite("/tmp/level.out");
	    if(level) {
	      try { 
		Level l(level); 
		levelwrite << l;
		Universe u(l);
		Spacehero s(display,u);
		if(s.play() == Spacehero::spacehero_exit) break;
	      } catch (Error::ParseLevel e) {
		std::cerr << e.msg() << std::endl;
	      }
	    }
	  }
	}
      }
#if 0
      ButtonFlags bflags;
      ButtonMaster mainmenu(*display.getPictureBook(), *display.getIllustrator());
      SDL_Event event;
      int width = (*display.getDisplay()).getWidth();
      int height = (*display.getDisplay()).getHeight();
      printf("%i x %i\n", width, height);
      mainmenu.addButton("button_start", 100, height/2, height*0.1, ButtonFlags::startGame);

      do
      {
      printf("%i x %i\n", width, height);
        while(SDL_PollEvent( &event ))
	{
	  (*display.getDisplay()).handleEvents(event);
          if(event.type == SDL_MOUSEBUTTONDOWN)
	  {
  	    mainmenu.checkButtons(bflags,event.motion.x,event.motion.y);
          }
	}
        
        (*display.getDisplay()).initDisplay();	
        mainmenu.drawButtons(); 	
        SDL_GL_SwapBuffers();
      } while(!bflags.isFlag());

      if(bflags.checkFlag(ButtonFlags::startGame) || bflags.viewFlag(ButtonFlags::startEditor))
      {
	  }
	}
      }
      
      if(bflags.checkFlag(ButtonFlags::chooseLevel))
      {

      }
      /*
      Menu mainmenu(display.getIllustrator(), display.getDisplay());
      mainmenu.add("START",&start);
      mainmenu.add("EDITOR",&editor);
      mainmenu.start();*/
#endif
    }
  } else {
    std::ifstream level(argv[1]);
    Level l(level);
    std::cerr << l << std::endl;
    Universe u(l);
    Spacehero s(display,u);
    s.play();
  }



  return 0;
}


