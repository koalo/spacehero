#ifndef _FILEMANAGER_H_
#define _FILEMANAGER_H_

#include <iostream>
#include "GLdisplay.h"
#include "Illustrator.h"
#include <ctype.h>

class FileManager
{
  private:
    std::string name;
    Illustrator &illustrator;
    GLdisplay &display;
    bool doinput;
  public:
    FileManager(Illustrator &illu, GLdisplay &disp) : name(""), illustrator(illu), display(disp), doinput(true) {}
    std::string getFile();
    void draw();
    void handleEvents();
};

#endif
