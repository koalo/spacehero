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
#include <ostream>
#include <iterator>
using namespace std;

#include <SDL.h>
#include <math.h>

#include "Constants.h"

Level::Level(ifstream &in) :
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
  ios_base::iostate s = in.exceptions();
  in.exceptions(ios_base::badbit|ios_base::failbit|ios_base::eofbit);
  in >> c >> i;
  if(c!='V' && i!=1) throw Error::ParseLevel("wrong version in level file");
  try {
    while( in >> c, in.good()) {
      cerr << "level: trying to read: " << c << endl;
      switch(c) {
        case 'D': 
          goal = Goal(in);
          D=true;
          break;
        case 'G': 
          {
            Galaxy g(in);
            galaxies.push_back( g );
            cerr << "master galaxy? " << g.getmaster() << endl;
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
          cerr << "unexpected char: " << c << endl;
          throw Error::ParseLevel("funny data in level");
      }
      cerr << "---" << endl;
    }
  } catch (ios_base::failure) {
    ios_base::iostate r = in.exceptions();
    if(r&ios_base::eofbit) {
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

Goal::Goal(ifstream &in)
{
  bool P=false,R=false;
  char c;
  bool done=false;
  while( !done) {
    in >> c, in.good();
    cerr << "goal: trying to read: " << c << endl;
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
        cerr << "unexpected char: " << c << endl;
        throw Error::ParseLevel("funny data in goal");
    }
  }
  if(!P) throw Error::ParseLevel("Missing Position for Goal");
  if(!R) throw Error::ParseLevel("Missing Radius for Goal");
  setlevel();
};

Blackhole::Blackhole(ifstream &in) {
  bool P=false,W=false;
  char c;
  bool done=false;
  while( !done) {
    in >> c, in.good();
    cerr << "hole: trying to read: " << c << endl;
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
        cerr << "unexpected char: " << c << endl;
        throw Error::ParseLevel("funny data in hole");
    }
  }
  if(!P) throw Error::ParseLevel("Missing Position for Hole");
  if(!W) throw Error::ParseLevel("Missing Weight for Hole");
  radius = HOLESIZE*sqrt(mass/HOLEMEDIUMMASS);
  setlevel();
};

Galaxy::Galaxy(ifstream &in) :
  master(false),
  lr(true)
{
  bool P=false,V=false,W=false;
  char c;
  bool done=false;
  while( !done) {
    in >> c, in.good();
    cerr << "galaxy: trying to read: " << c << endl;
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
        cerr << "Setting Galaxy as master" << endl;
        break;
      case 'R':
        lr=false;
        cerr << "Setting Galaxy as right" << endl;
        break;
      case 'W':
        in >> mass;
        W=true;
        break;
      default:
        cerr << "unexpected char: " << c << endl;
        throw Error::ParseLevel("funny data in galaxy");
    }
  }
  if(!P) throw Error::ParseLevel("Missing Position for Galaxy");
  if(!V) throw Error::ParseLevel("Missing Velocity for Galaxy");
  if(!W) throw Error::ParseLevel("Missing Weight for Galaxy");
  if(getmaster()) cerr << "this galaxy thinks it is master" << endl;
 
  setlevel();
  radius = BULGESIZE;
};
    
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
