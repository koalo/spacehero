#include "FileManager.h"

std::string FileManager::getFile()
{
  std::cout << "Save as: ";
  std::cin >> name;
  return name;
}
