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
#include "FileManager.h"

std::string FileManager::getFile(SpaceDisplay &disp, Universe &uni)
{
  name = "";
  doinput = true;
  /*std::cout << "Save as: ";
  std::cin >> name;*/
  int i = 0;
  while(doinput)
  {
    i++;
    draw(i,disp,uni);
    handleEvents(disp);
    usleep(100000);
  }
  return name;
}

void FileManager::draw(int i, SpaceDisplay &display, Universe &universe)
{ 
  display.getDisplay()->cleanDisplay();
  display.drawBridge(universe, SpaceDisplay::EditorView, 100);
  display.getDisplay()->OrthoMode(); 

  std::string sname = "Save this Level as " + name;
  if((i/2) % 2)
  {
    sname = sname + "_";
  }
  display.getIllustrator()->glPrint(30.0, 0.0, 1.0, 1.0, 10.0, 10.0, sname.c_str());
  SDL_GL_SwapBuffers();
}

void FileManager::handleEvents(SpaceDisplay &display)
{
  SDL_Event event;
  
  while ( SDL_PollEvent( &event ) )
  {
    display.getDisplay()->handleEvents(event);
    switch( event.type )
    {
      case SDL_KEYDOWN:
        if((event.key.keysym.sym >= 'a' && event.key.keysym.sym <= 'z') || (event.key.keysym.sym >= '0' && event.key.keysym.sym <= '9'))
        {
          name += toupper(event.key.keysym.sym);
        } 
	else if (event.key.keysym.sym == SDLK_RETURN)
        {
          doinput = false;
        }
        break;
      default:
        break;
    }
  }
}

void FileManager::loadLevels()
{
  for(std::vector<std::string>::iterator dir = dirs.begin(); dir != dirs.end(); dir++)
  {
    if (is_directory(*dir))
    {
      for (directory_iterator level(*dir); level != directory_iterator(); ++level)
      {
	std::ifstream levelstream(level->path().string().c_str());
	levels.push_back(Level(levelstream));
      }
    }
  }
}

Level FileManager::nextLevel()
{
  int nr = rand() % levels.size();
  Level ret = levels.at(nr);
  levels.erase(levels.begin()+nr);
  return ret;
}

bool FileManager::hasLevel()
{
  return (levels.size() > 0);
}
