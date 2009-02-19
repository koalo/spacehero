
#include "Universe.h"

std::ostream& operator<< (std::ostream &o, const SkyObject &g) {
//  o << "SkyObject: " << std::endl;
  o << "P " << g.x << " " << g.y << " " << g.z << std::endl;
  return o;
}

std::ostream& operator<< (std::ostream &o, const SkyMass &g) {
  //o << "SkyMass: " << std::endl;
  //o << "(fx,fy,fz): " << g.fx << " " << g.fy << " " << g.fz << std::endl;
  o << static_cast<SkyObject>(g);
  o << "V " << g.vx << " " << g.vy << " " << g.vz << std::endl;
  o << "W " << g.mass << std::endl;
  return o;
}
std::ostream& operator<< (std::ostream &o, const Goal &g) {
  o << "D " << std::endl;
  o << static_cast<SkyObject>(g);
  o << "R " << g.radius << std::endl;
  o << "D " << std::endl << std::endl;
  return o;
}
std::ostream& operator<< (std::ostream &o, const Blackhole &g) {
  o << "H" << std::endl;
  o << static_cast<SkyMass>(g);
  o << "H" << std::endl << std::endl;
  return o;
}
std::ostream& operator<< (std::ostream &o, const Star &g) {
  o << "S" << std::endl;
  o << static_cast<SkyMass>(g);
  o << "S" << std::endl << std::endl;
  return o;
}
std::ostream& operator<< (std::ostream &o, const Galaxy &g) {
  o << "G" << std::endl;
  o << (g.master?"M ":"") << std::endl;
  o << static_cast<SkyMass>(g);
  o << "G" << std::endl << std::endl;
  return o;
}

std::ostream& operator<< (std::ostream &o, const Level &l) {
  o << "V 1" << std::endl;
  o << "T " << l.getmaxtime() << std::endl;
  o << l.goal;
  copy(l.holes.begin(),l.holes.end(),std::ostream_iterator<Blackhole>(o));
  copy(l.galaxies.begin(),l.galaxies.end(),std::ostream_iterator<Galaxy>(o));
  return o;
};
