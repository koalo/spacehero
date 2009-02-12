
#include "Universe.h"



std::ostream& operator<< (std::ostream &o, const SkyObject &g) {
  o << "SkyObject: " << std::endl;
  o << "(x,y,z): " << g.x << " " << g.y << " " << g.z << std::endl;
  return o;
}

std::ostream& operator<< (std::ostream &o, const SkyMass &g) {
  o << "SkyMass: " << std::endl;
  o << "mass: " << g.mass << std::endl;
  o << "(fx,fy,fz): " << g.fx << " " << g.fy << " " << g.fz << std::endl;
  o << static_cast<SkyObject>(g) << std::endl;
  return o;
}
std::ostream& operator<< (std::ostream &o, const SkyMovableObject &g) {
  o << "SkyMovableObject: " << std::endl;
  o << "(vx,vy,vz)" << g.vx << " " << g.vy << " " << g.vz << std::endl;
  o << static_cast<SkyObject>(g) << std::endl;
  return o;
}
std::ostream& operator<< (std::ostream &o, const Goal &g) {
  o << "Goal: " << std::endl;
  o << "radius: " << g.radius << std::endl;
  o << static_cast<SkyObject>(g) << std::endl;
  return o;
}
std::ostream& operator<< (std::ostream &o, const Blackhole &g) {
  o << "Blackhole: " << std::endl;
  o << static_cast<SkyMass>(g) << std::endl;
  return o;
}
std::ostream& operator<< (std::ostream &o, const Star &g) {
  o << "Star: " << std::endl;
  o << static_cast<SkyMovableObject>(g) << std::endl;
  o << static_cast<SkyMass>(g) << std::endl;
  return o;
}
std::ostream& operator<< (std::ostream &o, const Galaxy &g) {
  o << "Galaxy: " << std::endl;
  o << static_cast<SkyMovableObject>(g) << std::endl;
  o << static_cast<SkyMass>(g) << std::endl;
  return o;
}
std::ostream& operator<< (std::ostream &o, const Level &l) {
  o << l.goal << std::endl;
  copy(l.holes.begin(),l.holes.end(),std::ostream_iterator<Blackhole>(o));
  copy(l.galaxies.begin(),l.galaxies.end(),std::ostream_iterator<Galaxy>(o));
  return o;
};
