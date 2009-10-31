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
#ifndef _PICTUREBOOK_H_
#define _PICTUREBOOK_H_

#include <string>
#include <map>
using namespace std;

class PictureBook
{
  private:
    string path;
    map<string, unsigned int> textureMap;

  public:
    PictureBook(string texturepath);
    int addTexture(string texture);
    bool isTexture(string texture);
    int useTexture(string texture);
    void noTexture();
};

#endif
