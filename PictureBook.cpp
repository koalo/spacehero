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

  if( (textureImage  = IMG_Load( (path + texture + ".png").c_str() )) )
  {
    /* Textur einstellen */
    glBindTexture( GL_TEXTURE_2D, textureName );

    /* Filter */
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    /* Textur erstellen */
    if(textureImage->format->BytesPerPixel == 4)
    {
      glTexImage2D( GL_TEXTURE_2D, 0, 4, textureImage->w, textureImage->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureImage->pixels );
    } else {
      glTexImage2D( GL_TEXTURE_2D, 0, 3, textureImage->w, textureImage->h, 0, GL_RGB, GL_UNSIGNED_BYTE, textureImage->pixels );
    }
 
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

bool PictureBook::isTexture(std::string texture)
{
  std::map<std::string, GLuint>::iterator iter = textureMap.find(texture);
  if( iter != textureMap.end() )
  {
    return true;
  } else {
    return false;
  }
}

void PictureBook::noTexture()
{
  glBindTexture( GL_TEXTURE_2D, 0 );
}
