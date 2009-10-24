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
#ifndef _FILEMANAGER_H_
#define _FILEMANAGER_H_

#include <iostream>
#include "SpaceDisplay.h"
#include <ctype.h>
#include <vector>

#include "boost/filesystem.hpp"
#include "boost/regex.hpp"
using namespace boost::filesystem;

class FileManager
{
  private:
    FileManager(const FileManager&);
    FileManager& operator=(const FileManager&);
  private:
    std::vector<std::string> dirs;
    std::string savedir;
  private:
    std::string name;
    bool doinput;
    std::vector<Level> levels;
  public:
    FileManager() : dirs(), savedir(""), name(""), doinput(true), levels() {}
    void addLevelDir(std::string dir){dirs.push_back(dir);}
    void setSaveDir(std::string dir){savedir = dir;}
    void loadLevels();
    Level nextLevel();
    bool hasLevel();
    std::string getFile(SpaceDisplay &disp, Universe &uni);
    void handleEvents(SpaceDisplay &display);
    void LevelMan(SpaceDisplay& display);
  private:
    void draw(int i, SpaceDisplay &display, Universe &universe);
    void drawList(SpaceDisplay &display, float fontsize, int active, ButtonMaster& buttons);
};
#endif

