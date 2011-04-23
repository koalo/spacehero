
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
#ifndef _TIME_H_
#define _TIME_H_

#include <SDL.h>

using namespace std;


class Time 
{
	public:
		Uint32 now();
		virtual ~Time() {};
};

class GameTime : public Time
{
		Uint32 mtime;
	public:
		GameTime();
		Uint32 elapsed();
};

class RealTime : public Time 
{
		Uint32 mtime;
	public:
		RealTime();
		Uint32 elapsed();
};


#endif
