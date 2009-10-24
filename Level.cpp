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
#include <fstream>

Level::Level(std::ifstream &in) :
  t0(),
  maxtime(30.0),
  lastt(0),
  m_delta(0),
  m_fpst(0),
  m_fps(0),
  name(""),
  holes(),
  goal()
{
  char c; int i;
  bool D=false,G=false;
  std::ios_base::iostate s = in.exceptions();
  in.exceptions(std::ios_base::badbit|std::ios_base::failbit|std::ios_base::eofbit);
  in >> c >> i;
  if(c!='V' && i!=1) throw Error::ParseLevel("wrong version in level file");
  try {
    while( in >> c, in.good()) {
      std::cerr << "level: trying to read: " << c << std::endl;
      switch(c) {
        case 'D': 
          goal = Goal(in);
          D=true;
          break;
        case 'G': 
          {
            Galaxy g(in);
            galaxies.push_back( g );
            std::cerr << "master galaxy? " << g.getmaster() << std::endl;
            if(g.getmaster()) G=true; // at least one master galaxy needed
          }
          break;
        case 'H':
          holes.push_back( Blackhole(in) );
          break;
        case 'S':
          in >> seed;
          break;
        case 'T':
          in >> maxtime;
          break;
        default:
          std::cerr << "unexpected char: " << c << std::endl;
          throw Error::ParseLevel("funny data in level");
      }
      std::cerr << "---" << std::endl;
    }
  } catch (std::ios_base::failure) {
    std::ios_base::iostate r = in.exceptions();
    if(r&std::ios_base::eofbit) {
      // end of level reached
      if(!(D&&G)) {
        if (!D) throw Error::ParseLevel("at least one goal needed");
        if (!G) throw Error::ParseLevel("at least one master galaxy needed");
      }
    } else {
      throw;
    }
  }
  in.exceptions(s);
}

Goal::Goal(std::ifstream &in)
{
  bool P=false,R=false;
  char c;
  bool done=false;
  while( !done) {
    in >> c, in.good();
    std::cerr << "goal: trying to read: " << c << std::endl;
    switch(c) {
      case 'D':
        done=true;
        break;
      case 'P': 
        in >> x >> y >> z;
        P=true;
        break;
      case 'R': 
        in >> radius;
        R=true;
        break;
      default:
        std::cerr << "unexpected char: " << c << std::endl;
        throw Error::ParseLevel("funny data in goal");
    }
  }
  if(!P) throw Error::ParseLevel("Missing Position for Goal");
  if(!R) throw Error::ParseLevel("Missing Radius for Goal");
  setlevel();
};

Blackhole::Blackhole(std::ifstream &in) {
  bool P=false,W=false;
  char c;
  bool done=false;
  while( !done) {
    in >> c, in.good();
    std::cerr << "hole: trying to read: " << c << std::endl;
    switch(c) {
      case 'H':
        done=true;
        break;
      case 'P': 
        in >> x >> y >> z;
        P=true;
        break;
      case 'V':
        double t;
        in >> t >> t >> t; 
        break;
      case 'W': 
        in >> mass;
        W=true;
        break;
      default:
        std::cerr << "unexpected char: " << c << std::endl;
        throw Error::ParseLevel("funny data in hole");
    }
  }
  if(!P) throw Error::ParseLevel("Missing Position for Hole");
  if(!W) throw Error::ParseLevel("Missing Weight for Hole");
  radius = HOLESIZE*sqrt(mass/HOLEMEDIUMMASS);
  setlevel();
};

Galaxy::Galaxy(std::ifstream &in) :
  master(false),
  lr(true)
{
  bool P=false,V=false,W=false;
  char c;
  bool done=false;
  while( !done) {
    in >> c, in.good();
    std::cerr << "galaxy: trying to read: " << c << std::endl;
    switch(c) {
      case 'G':
        done=true;
        break;
      case 'P': 
        in >> x >> y >> z;
        P=true;
        break;
      case 'V': 
        in >> vx >> vy >> vz; 
        V=true;
        break;
      case 'M':
        master=true;
        std::cerr << "Setting Galaxy as master" << std::endl;
        break;
      case 'R':
        lr=false;
        std::cerr << "Setting Galaxy as right" << std::endl;
        break;
      case 'W':
        in >> mass;
        W=true;
        break;
      default:
        std::cerr << "unexpected char: " << c << std::endl;
        throw Error::ParseLevel("funny data in galaxy");
    }
  }
  if(!P) throw Error::ParseLevel("Missing Position for Galaxy");
  if(!V) throw Error::ParseLevel("Missing Velocity for Galaxy");
  if(!W) throw Error::ParseLevel("Missing Weight for Galaxy");
  if(getmaster()) std::cerr << "this galaxy thinks it is master" << std::endl;
 
  setlevel();
  radius = BULGESIZE;
};
