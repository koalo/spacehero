#ifndef _PICTUREBOOK_H_
#define _PICTUREBOOK_H_

#include <GL/gl.h>
#include <SDL.h>
#include <SDL_image.h>
#include <map>
#include <iostream>

class PictureBook
{
  private:
    std::string path;
    std::map<std::string, GLuint> textureMap;

  public:
    PictureBook(std::string texturepath);
    int addTexture(std::string texture);
    int useTexture(std::string texture);
    void noTexture();
};

#endif
