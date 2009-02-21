/* 
 * This file is part of Spacehero.
 * 
 * Spacehero is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Foobar is distributed in the hope that it will be useful,
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
#include "GLdisplay.h"
#include "Illustrator.h"
#include <ctype.h>

class FileManager
{
  private:
    FileManager(const FileManager&);
    FileManager& operator=(const FileManager&);
  private:
    std::string name;
    Illustrator *illustrator;
    GLdisplay *display;
    bool doinput;
  public:
    FileManager(Illustrator *illu, GLdisplay *disp) : name(""), illustrator(illu), display(disp), doinput(true) {}
    std::string getFile();
    void draw();
    void handleEvents();
};

#endif
