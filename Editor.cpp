#include "Editor.h"

Editor::Editor(Universe &universe, bool allowall) : uni(universe),maxreserve(MAXSTARTRESERVE),all(allowall),massreserve(maxreserve),size(medium)
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
        if(!all) massreserve += uni.holes[i].mass;
        uni.holes.erase(uni.holes.begin()+i);
      }
    }
  }
  
  if(!remove && (massreserve >= getHoleWeight() || all))
  {
    if(!all) massreserve -= getHoleWeight();
    uni.holes.push_back(Blackhole(mousex,mousey,getHoleWeight()));
  }
}

double Editor::getQuotient()
{
  return massreserve/maxreserve;
}

double Editor::getHoleWeight()
{
  switch(size)
  {
    case small:
      return HOLESMALLMASS;
      break;
    case medium:
      return HOLEMEDIUMMASS;
      break;
    case large:
      return HOLELARGEMASS;
      break;
    default:
      return 0;
      break;
  }
}

void Editor::parseButtons(ButtonFlags &flags)
{
  if(flags.checkFlag(ButtonFlags::small))
  {
    size = small;
  }

  if(flags.checkFlag(ButtonFlags::medium))
  {
    size = medium;
  }

  if(flags.checkFlag(ButtonFlags::large))
  {
    size = large;
  }
}
