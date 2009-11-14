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

#include <vector>
using namespace std;

#include "SpaceDisplay.h"
#include "Universe.h"

class FileManager
{
  private:
    FileManager(const FileManager&);
    FileManager& operator=(const FileManager&);
    vector<string> dirs;
    string savedir;
    string name;
    bool doinput;
    bool nameinput; 
    vector<Level> levels;
    void draw(int i, SpaceDisplay &display, Universe &universe);
    void drawList(SpaceDisplay &display, float fontsize, int active, ButtonMaster& buttons);
  public:
    FileManager();
    void addLevelDir(string dir);
    void setSaveDir(string dir);
    void loadLevels();
    Level nextLevel();
    bool hasLevel();
    string getFile(SpaceDisplay &disp, Universe &uni);
    void handleEvents(SpaceDisplay &display);
    void LevelMan(SpaceDisplay& display);
};
#endif

