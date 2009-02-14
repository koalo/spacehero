
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
  path.push_back( "./" );
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
        if(level) {
          Level l(level);
          std::cerr << l << std::endl;
          Universe u(l);
          Spacehero s(display,u);
          s.play();

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


