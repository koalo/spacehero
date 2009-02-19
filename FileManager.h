#ifndef _FILEMANAGER_H_
#define _FILEMANAGER_H_

#include <iostream>

class FileManager
{
  private:
    std::string name;
  public:
    FileManager() : name(""){}
    std::string getFile();
};

#endif
