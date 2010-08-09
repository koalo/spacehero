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

#include <GL/gl.h>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/regex.hpp>
using namespace boost::filesystem;

#include "Spacehero.h"
#include "HttpManager.h"

FileManager::FileManager()
  : dirs(), savedir(""), position(0), nameinput(false), levels() 
{
  savedir = getenv("HOME");
  savedir += "/.spacehero";
}
    
void FileManager::addLevelDir(string dir)
{
  dirs.push_back(dir);
}
/*
void FileManager::setSaveDir(string dir)
{
  savedir = dir;
}
*/
void FileManager::saveLevel(Level level)
{
  if(level.getFilename() != "")
  {
    if(!exists( savedir ))
    {
      create_directory( savedir );
    }

    string savefile = savedir+"/"+level.getFilename()+".txt";
    cout << "Wird jetzt gespeichert in: " << savefile << endl;
    boost::filesystem::ofstream levelwrite;
    levelwrite.exceptions ( boost::filesystem::ofstream::eofbit | boost::filesystem::ofstream::failbit | boost::filesystem::ofstream::badbit );
    levelwrite.open(savefile.c_str());
    levelwrite << level;
    levelwrite.close();
  }
}

/*
string FileManager::getFile(SpaceDisplay &disp, Universe &uni)
{
  name = "";
  doinput = true;
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
	else if(event.key.keysym.sym == SDLK_BACKSPACE && name.length() > 0)
	{
	  name.erase(name.end()-1);
	}
	else if(event.key.keysym.sym == SDLK_BACKSPACE || event.key.keysym.sym == SDLK_ESCAPE)
	{
          name = "";
	  doinput = false;
	}
	else if(event.key.keysym.sym == SDLK_RETURN)
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

  string sname = "Save this Level as " + name;
  if((i/2) % 2)
  {
    sname = sname + "_";
  }
  display.getIllustrator()->glPrint(10.0, 10.0, sname.c_str());
  SDL_GL_SwapBuffers();
}
*/
void FileManager::loadLevels()
{
  string name;
  const boost::regex levelname("^(.*)\\.[A-Za-z0-9]*$");
  levels.clear();
  for(vector<string>::iterator dir = dirs.begin(); dir != dirs.end(); dir++)
  {
    if (is_directory(*dir))
    {
      for (directory_iterator level(*dir); level != directory_iterator(); ++level)
      {
	boost::filesystem::ifstream levelstream(level->path().string().c_str());
	levels.push_back(Level(levelstream));
	name = level->path().leaf();  
        name = boost::regex_replace(name, levelname, "\\1", boost::match_default | boost::format_sed);
	levels.back().setFilename(name);
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
      display.getIllustrator()->handleInput(event);

      if(event.key.keysym.sym == SDLK_ESCAPE)
      {
        nameinput = false;
      }

      if(event.type == SDL_MOUSEBUTTONDOWN)
      {
	buttons.checkButtons(flags,event.motion.x,event.motion.y);
	nr = (unsigned int)(event.motion.y / fontsize);
	if(event.motion.x < display.getDisplay()->getWidth()*0.8 && event.motion.x > display.getDisplay()->getWidth()*0.1 && nr > 0 && nr < levels.size()+1)
	{
	  active = ((nr-1)+position)%levels.size();
	  cerr << levels.at(active).getName() << endl;
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
      display.getIllustrator()->startInput("Transfers this level to spacehero.de\nENTER for transfer, ESC for abort\n\nYour Name (optional):");
      nameinput = true;
    }
    
    if(flags.checkFlag(ButtonFlags::up))
    {
      position--;
      if(position < 0) position = levels.size();
    }
    
    if(flags.checkFlag(ButtonFlags::down))
    {
      position++;
      if(position > (int)levels.size()) position = 0;
    }
    
    if(nameinput && !display.getIllustrator()->doingInput())
    {
      nameinput = false;
      cout << "Transfer wurde aufgerufen" << endl;
      drawList(display,fontsize,active,buttons);
      try
      {
	HttpManager http("localhost");
	http << "level=";
	http << levels.at(active); 
	if(display.getIllustrator()->getInput() != "")
	{
	  http << "&creator=";
	  http << display.getIllustrator()->getInput();
	}
	http << "&title=";
	http << levels.at(active).getFilename();
	//float fs = 50;
	//string text;
	http.send("/recvLevel.php");
	display.getIllustrator()->drawMessage("OK\nYour Level is transferred to spacehero.de\nIt will appear there in some minutes.");
      } 
      catch(exception &e)
      {
	display.getIllustrator()->drawMessage(string("FAILED\n")+e.what());
	cerr << e.what() << endl;
      }
      //display.getIllustrator()->setFontheight(fs);
      SDL_GL_SwapBuffers();
      display.getDisplay()->waitForUser();
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
  display.getIllustrator()->setFontalign(Illustrator::NORTH, Illustrator::WEST);
  //display.getPictureBook()->noTexture();
    
  glDisable(GL_BLEND);
  glColor4f(1,1,1,1);
  display.getIllustrator()->setFontheight(fontsize*0.8);
  for(unsigned int i = 0; i < 13 && i < levels.size(); i++)
  {
    y = (i+1)*fontsize;
    if((int)(i+position)%levels.size() == active) display.getIllustrator()->drawRect(0, 0, 1, display.getDisplay()->getWidth()*0.09, y, display.getDisplay()->getWidth()*0.7,fontsize);
    glColor3f(1.0,1.0,0.0);
    display.getIllustrator()->glPrint(display.getDisplay()->getWidth()*0.12, y+fontsize*0.1, levels.at((i+position)%levels.size()).getName().c_str());
    glColor3f(1.0,1.0,1.0);
  }

  buttons.clearButtons();
  buttons.addButton("START", display.getDisplay()->getWidth()*0.9, display.getDisplay()->getHeight()*(1/8.0), display.getDisplay()->getWidth()*0.06, ButtonFlags::startGame);
  buttons.addButton("EDIT", display.getDisplay()->getWidth()*0.9, display.getDisplay()->getHeight()*(3/8.0), display.getDisplay()->getWidth()*0.06, ButtonFlags::startEditor);
  /*
  buttons.addButton("TRANSFER", display.getDisplay()->getWidth()*0.9, display.getDisplay()->getHeight()*(5/8.0), display.getDisplay()->getWidth()*0.06, ButtonFlags::transfer);
  */
  buttons.addButton("button_x", display.getDisplay()->getWidth()*0.9, display.getDisplay()->getHeight()*(7/8.0), display.getDisplay()->getWidth()*0.06, ButtonFlags::exit);
  buttons.addButton("button_up", display.getDisplay()->getWidth()*0.05, display.getDisplay()->getHeight()*0.11, display.getDisplay()->getWidth()*0.03, ButtonFlags::up);
  buttons.addButton("button_down", display.getDisplay()->getWidth()*0.05, display.getDisplay()->getHeight()*0.91, display.getDisplay()->getWidth()*0.03, ButtonFlags::down);
  buttons.drawButtons();  

  display.getIllustrator()->drawInput();
  SDL_GL_SwapBuffers();
}

