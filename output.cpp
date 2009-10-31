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

#include "Universe.h"

#include <ostream>
#include <iterator>
using namespace std;

ostream& operator<< (ostream &o, const SkyObject &g) {
//  o << "SkyObject: " << endl;
  o << "P " << g.x << " " << g.y << " " << g.z << endl;
  return o;
}

ostream& operator<< (ostream &o, const SkyMass &g) {
  //o << "SkyMass: " << endl;
  //o << "(fx,fy,fz): " << g.fx << " " << g.fy << " " << g.fz << endl;
  o << static_cast<SkyObject>(g);
  o << "V " << g.vx << " " << g.vy << " " << g.vz << endl;
  o << "W " << g.mass << endl;
  return o;
}
ostream& operator<< (ostream &o, const Goal &g) {
  o << "D " << endl;
  o << static_cast<SkyObject>(g);
  o << "R " << g.radius << endl;
  o << "D " << endl << endl;
  return o;
}
ostream& operator<< (ostream &o, const Blackhole &g) {
  o << "H" << endl;
  o << static_cast<SkyMass>(g);
  o << "H" << endl << endl;
  return o;
}
ostream& operator<< (ostream &o, const Star &g) {
  o << "S" << endl;
  o << static_cast<SkyMass>(g);
  o << "S" << endl << endl;
  return o;
}
ostream& operator<< (ostream &o, const Galaxy &g) {
  o << "G" << endl;
  o << (g.master?"M ":"") << endl;
  o << static_cast<SkyMass>(g);
  o << "G" << endl << endl;
  return o;
}

ostream& operator<< (ostream &o, const Level &l) {
  o << "V 1" << endl;
  o << "T " << l.getmaxtime() << endl;
  o << l.goal;
  copy(l.holes.begin(),l.holes.end(),ostream_iterator<Blackhole>(o));
  copy(l.galaxies.begin(),l.galaxies.end(),ostream_iterator<Galaxy>(o));
  return o;
};
