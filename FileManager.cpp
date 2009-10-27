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

    SDL_Event event;
    while ( SDL_PollEvent( &event ) )
    {
      disp.getDisplay()->handleEvents(event);
      if(event.type == SDL_KEYDOWN)
      {
       	if((event.key.keysym.sym >= 'a' && event.key.keysym.sym <= 'z') || (event.key.keysym.sym >= '0' && event.key.keysym.sym <= '9'))
        {
	  name += toupper(event.key.keysym.sym);
	} 
	else if (event.key.keysym.sym == SDLK_RETURN)
	{
	  doinput = false;
	}
      }
    }

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

void FileManager::loadLevels()
{
  std::string name;
  const boost::regex levelname("^(.*)\\.[A-Za-z0-9]*$");
  levels.clear();
  for(std::vector<std::string>::iterator dir = dirs.begin(); dir != dirs.end(); dir++)
  {
    if (is_directory(*dir))
    {
      for (directory_iterator level(*dir); level != directory_iterator(); ++level)
      {
	std::ifstream levelstream(level->path().string().c_str());
	levels.push_back(Level(levelstream));
	name = level->path().leaf();  
        name = boost::regex_replace(name, levelname, "\\1", boost::match_default | boost::format_sed);
	levels.back().setName(name);
      }
    }
  }
  srand(time(NULL));
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

void FileManager::LevelMan(SpaceDisplay& display)
{
  unsigned int nr;
  int active = -1;
  Level l;
  float fontsize = 40.0;
  ButtonMaster buttons(*display.getPictureBook(), *display.getIllustrator());
  ButtonFlags flags = *(new ButtonFlags());

  while(true)
  {
    drawList(display,fontsize,active,buttons);
    SDL_Event event;
    while ( SDL_PollEvent( &event ) )
    {
      display.getDisplay()->handleEvents(event);
      if(event.type == SDL_MOUSEBUTTONDOWN)
      {
	buttons.checkButtons(flags,event.motion.x,event.motion.y);
	nr = (unsigned int)(event.motion.y / fontsize);
	if(event.motion.x < display.getDisplay()->getWidth()*0.8 && nr > 0 && nr < levels.size()+1)
	{
	  active = nr - 1;
	  std::cerr << levels.at(nr-1).getName() << std::endl;
	}
      }
    }

    if(flags.checkFlag(ButtonFlags::exit))
    {
      break;
    }

    if(flags.checkFlag(ButtonFlags::startGame) && active >= 0 && active < (int)levels.size())
    {
      l = levels.at(active);
      Universe uni(l);
      Spacehero space(display,uni);
      space.play();
    }

    if(flags.checkFlag(ButtonFlags::startEditor) && active >= 0 && active < (int)levels.size())
    {
      l = levels.at(active);
      Universe uni(l);
      Spacehero space(display,uni);
      space.play(SpaceDisplay::EditorView);
    }

    if(flags.checkFlag(ButtonFlags::transfer) && active >= 0 && active < (int)levels.size())
    {
    }

    usleep(100000);
  }
}

void FileManager::drawList(SpaceDisplay &display, float fontsize, int active, ButtonMaster& buttons)
{  
  float y;

  /* Bildschirm loeschen */
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );  
  display.getDisplay()->OrthoMode();
  
  //display.getPictureBook()->noTexture();
    
  glDisable(GL_BLEND);
  glColor4f(1,1,1,1);
  for(unsigned int i = 0; i < levels.size(); i++)
  {
    y = (i+1)*fontsize;
    if((int)i == active) display.getIllustrator()->drawRect(0, 0, 1, display.getDisplay()->getWidth()*0.03, y, display.getDisplay()->getWidth()*0.7,fontsize);
    display.getIllustrator()->glPrint(fontsize*0.8, 0.7, 0.7, 0, display.getDisplay()->getWidth()*0.05, y+fontsize*0.1, levels.at(i).getName().c_str());
  }

  buttons.clearButtons();
  buttons.addButton("button_red", display.getDisplay()->getWidth()*0.9, display.getDisplay()->getHeight()*(1/8.0), display.getDisplay()->getWidth()*0.06, ButtonFlags::startGame);
  buttons.addButton("button_green", display.getDisplay()->getWidth()*0.9, display.getDisplay()->getHeight()*(3/8.0), display.getDisplay()->getWidth()*0.06, ButtonFlags::startEditor);
  buttons.addButton("button_green", display.getDisplay()->getWidth()*0.9, display.getDisplay()->getHeight()*(5/8.0), display.getDisplay()->getWidth()*0.06, ButtonFlags::transfer);
  buttons.addButton("button_x", display.getDisplay()->getWidth()*0.9, display.getDisplay()->getHeight()*(7/8.0), display.getDisplay()->getWidth()*0.06, ButtonFlags::exit);
  buttons.drawButtons();  

  SDL_GL_SwapBuffers();
}

