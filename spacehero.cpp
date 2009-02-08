
#include "boost/filesystem.hpp"
#include <iostream>
#include <fstream>
using namespace boost::filesystem;

#include <stdlib.h>

#include "GLdisplay.h"
#include "intro.h"

int main(int argc, char *argv[])
{ 
	std::string levels("./level/");
	GLdisplay display;

	std::cerr << "argc: " << argc << std::endl;

	if(argc<=1) {
		std::cerr << "leveldir: "<< levels << std::endl;
		if (is_directory(levels) ) {
			std::cerr << "level dir found" << std::endl;

			intro(display);
			std::cerr << "intro done" << std::endl;

			for (directory_iterator itr(levels); itr != directory_iterator(); ++itr) {
				std::cerr << "trying to load level: " << itr->path() << std::endl;
				std::ifstream level(itr->path().string().c_str());
				if(level) display.startRound(level);
			}
		}
	} else {
		std::ifstream level(argv[1]);
		display.startRound(level);

	}

	return 0;
}


