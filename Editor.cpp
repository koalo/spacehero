#include "Editor.h"

Editor::Editor(Universe &universe) : uni(universe),massreserve(MAXSTARTRESERVE),holeWeight(HOLEMEDIUMMASS)
{
}

void Editor::check(double mousex, double mousey)
{
  unsigned int i, remove = 0;

  for(i = 0; i < uni.holes.size(); i++)
  {
    if( (pow(mousex-uni.holes[i].x,2)+pow(mousey-uni.holes[i].y,2))
	  <= pow(HOLESIZE*(sqrt(uni.holes[i].mass/HOLEMEDIUMMASS)),2) )
    {
      remove = 1;
      
      /* pruefen ob der ueberhaupt geloescht werden darf */
      if(!uni.holes[i].level)
      {
        massreserve += uni.holes[i].mass;
        uni.holes.erase(uni.holes.begin()+i);
      }
    }
  }
  
  if(!remove && massreserve >= holeWeight)
  {
    massreserve -= holeWeight;
    uni.holes.push_back(Blackhole(mousex,mousey,holeWeight));
  }
}