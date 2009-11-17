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

#ifndef LOCAL_H
#define LOCAL_H

#define YEARINSECONDS 365*24*60*60.0
#define TIMESCALE 1400000.0*YEARINSECONDS /* -> so viele Sekunden werden pro Schritt berechnet */
#define TIMESTEP 0.02 /* Sekunden zwischen zwei Berechnungen */

#define GRAVKONST 6.67428e-11
#define SUNMASS 1.9891e30
#define LJ 9.461e15
#define WIDTHINMETERS (600000.0*LJ)
/*#define WIDTHINMETERS 56766e17*/
#define WIDTHINMETERS2 3222378756.0e34

//#define SUNGRAV 13275810348.0e10
#define SUNGRAV (GRAVKONST*SUNMASS)
#define SUNGRAVTIME 5861323371883392.0e18
#define SUNGRAVTIMEWIDTH (SUNGRAVTIME/WIDTHINMETERS2)
//#define SUNGRAVTIMEWIDTH 1.81894303e-10

/* maximale Anzahl an... */ 
#define MAX_GALAXIES 50
#define MAX_PUT_HOLES 10
#define MAX_LEVEL_HOLES 50
#define MAX_STARS 2000

#define ORTHOGONAL 56345
#define PERSPECTIVE 34523

#define HOLESMALLMASS 8.0e11
#define HOLEMEDIUMMASS HOLESMALLMASS*3.0
#define HOLELARGEMASS HOLESMALLMASS*5.0
#define MAXSTARTRESERVE MAX_PUT_HOLES*HOLESMALLMASS

#define BULGEMEDIUMMASS 6e11
#define BULGESMALLMASS 0.6*BULGEMEDIUMMASS
#define BULGELARGEMASS 1.44*BULGEMEDIUMMASS

#define MEDIUMGOAL 0.08
#define SMALLGOAL 0.7*MEDIUMGOAL
#define LARGEGOAL 1.3*MEDIUMGOAL

#define MAXTIME 1000 /* in Timesteps */

#define BULGESIZE 0.015f
#define STARSIZE 0.0028f
#define HOLESIZE 0.015f

#define VORN 300
#define SPEED 0

/* Display-Konstanten */
#define UNIVERSE_LEFT 0
#define UNIVERSE_TOP 0
#define UNIVERSE_BOTTOM 0

#define PANELRATIO 0.205962059620
#define UNIVERSE_RIGHT (PANELRATIO*display.getHeight())

#define START_BUTTON (UNIVERSE_RIGHT*0.4)
#define REPLAY_BUTTON (UNIVERSE_RIGHT*0.2)
#define EXIT_BUTTON (UNIVERSE_RIGHT*0.1)
#define MARGIN 10

#define SMALL_HOLE (UNIVERSE_RIGHT*0.1)
#define MEDIUM_HOLE (UNIVERSE_RIGHT*0.12)
#define LARGE_HOLE (UNIVERSE_RIGHT*0.14)

#define VIEWANGLE 60

#define TEXTR 0.0f
#define TEXTG 1.0f
#define TEXTB 0.0f
#endif
