#include "PictureBook.h"

PictureBook::PictureBook(std::string texturepath)
  : path(texturepath), textureMap()
{
  std::cerr << "trying to load textures from: " << path << std::endl;
}

int PictureBook::addTexture(std::string texture)
{
  GLuint textureName;
  SDL_Surface *textureImage;
  
  glGenTextures( 1, &textureName );

  if( (textureImage  = IMG_Load( (path + "star.png").c_str() )) )
  {
    /* Textur einstellen */
    glBindTexture( GL_TEXTURE_2D, textureName );

    /* Filter */
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    /* Textur erstellen */
    glTexImage2D( GL_TEXTURE_2D, 0, 3, textureImage->w, textureImage->h, 0, GL_RGB, GL_UNSIGNED_BYTE, textureImage->pixels );
 
    SDL_FreeSurface(textureImage);

    textureMap.insert(std::pair<std::string, GLuint>(texture, textureName));

    return 1;
  } else {
    std::cerr << "error loading texture " << texture << " from:" << path << std::cerr;
    return 0;
  }
}

int PictureBook::useTexture(std::string texture)
{
  std::map<std::string, GLuint>::iterator iter = textureMap.find(texture);
  if( iter != textureMap.end() )
  {
    glBindTexture( GL_TEXTURE_2D, iter->second );
    return 1;
  } else {
    std::cerr << "texture " << texture << " was not load!" << std::cerr;
    return 0;
  }
}

void PictureBook::noTexture()
{
  glBindTexture( GL_TEXTURE_2D, 0 );
}
