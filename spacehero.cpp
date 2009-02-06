
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

	intro(display);
	std::cerr << "intro done" << std::endl;
	std::cerr << "argc: " << argc << std::endl;

	if(argc<=1)
	{
	std::cerr << "leveldir: "<< levels << std::endl;
		if (is_directory(levels) )
		{
	std::cerr << "level dir found" << std::endl;

			for (directory_iterator itr(levels); itr != directory_iterator(); ++itr) 
			{
				std::cerr << itr->path().filename() << std::endl;
				std::ifstream level(itr->path().filename().c_str());
				display.startRound(level);
			}
		}
	}
	else {
		std::ifstream level(argv[1]);
		display.startRound(level);

	}
	/*
	//char levelString[50];
	int rlevel = -1, altrlevel;


	srand((unsigned int) time(NULL));

	//  SDL_ShowCursor(SDL_DISABLE);
	//  intro(&display);
	//SDL_ShowCursor(SDL_ENABLE);

	do
	{
	if(argc == 1)
	{
	altrlevel = rlevel;
	do
	{
	rlevel = (int)((rand() / (RAND_MAX + 1.0))*LEVEL)+1;
	} while(rlevel == altrlevel);

	sprintf(levelString,"level%i",rlevel);
	} else {
	strcpy(levelString,argv[1]);
	}

	startRound(&display,levelString);
	} while(rlevel != -1 && !display.state.exit);
	*/

	return 0;
}


