
#include "boost/filesystem.hpp"
#include <iostream>
#include <fstream>
using namespace boost::filesystem;

#include <stdlib.h>

#include "SpaceDisplay.h"
#include "Spacehero.h"
#include "Universe.h"
#include "intro.h"

int main(int argc, char *argv[])
{ 
  std::string levels("./level/");
  SpaceDisplay display;

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
          Universe u(l);
          Spacehero s(display,u);


        }
      }
    }
  } else {
    std::ifstream level(argv[1]);
    Level l(level);
    std::cerr << l << std::endl;
    Universe u(l);
    Spacehero s(display,u);

  }



  return 0;
}


