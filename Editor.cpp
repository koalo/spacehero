#include "Editor.h"

void Editor::check(double mousex, double mousey, Universe &uni)
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