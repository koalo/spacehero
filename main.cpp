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

int main(int argc, char *argv[])
{ 
  std::vector<std::string> path;

#ifdef DEV
  path.push_back( "./" );
#endif
  path.push_back( "~/.spacehero/" );
  path.push_back( "/usr/share/games/spacehero/" );

  std::string dir;
  for(std::vector<std::string>::iterator l = path.begin(); l!=path.end(); l++) {
    std::cerr << "trying: " << *l << std::endl;
    if (is_directory(dir = *l) ) {
      std::cerr << "found: " << dir << std::endl;
      break;
    }
  }


  SpaceDisplay display(dir+"data/");
  std::string levels = dir+"level/";

  std::cerr << "argc: " << argc << std::endl;

  if(argc<=1) {
    std::cerr << "leveldir: "<< levels << std::endl;
    if (is_directory(levels) ) {
      std::cerr << "level dir found" << std::endl;

      //intro(display);
      std::cerr << "intro done" << std::endl;

      for (directory_iterator itr(levels); itr != directory_iterator(); ++itr) {
        std::cerr << "trying to load level: " << itr->path() << std::endl;
        std::ifstream level(itr->path().string().c_str());
        std::ofstream levelwrite("/tmp/level.out");
        if(level) {
          try { 
            Level l(level); 
            levelwrite << l;
            Universe u(l);
            Spacehero s(display,u);
            s.play();
          } catch (Error::ParseLevel e) {
            std::cerr << e.msg() << std::endl;
          }

        }
      }
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


