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
#ifndef BUTTONFLAGS_H
#define BUTTONFLAGS_H

class ButtonFlags : public AbstractButtonFlags
{
  public:
  enum Actions {
    small = 1,
    medium = 2,
    large = 4,
    startSimulation = 8,
    breakSimulation = 16,
    replaySimulation = 32,
    breakIntro = 64,
    exit = 128,
    putHole = 256,
    putBulge = 512,
    putGoal = 1024,
    startEditor = 2048,
    saveLevel = 4096,
    startGame = 8192,
    chooseLevel = 16384,
    skipLevel = 32768
  };
};

#endif
