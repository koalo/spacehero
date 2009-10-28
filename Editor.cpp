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
#include "Editor.h"

Editor::Editor(Universe &universe) : uni(universe),maxreserve(MAXSTARTRESERVE),all(false),massreserve(maxreserve),setGalaxy(false),galaxyX(0),galaxyY(0),canPut(false),removing(false),size(medium),type(hole),zoom(0),scissors(),pointer()
{
static const char *image[] = {
  /* width height num_colors chars_per_pixel */
  "    32    32        3            1",
  /* colors */
  "X c #000000",
  ". c #ffffff",
  "  c None",
  /* pixels */
"                                ",
"                                ",
"                                ",
"                                ",
"   .....               ....     ",
" ..XXXXX.             .XXXX.    ",
".XXXXXXXX.           .XXXXX.    ",
".XXXX..XXX.         .XXXXX.     ",
".XXX.  .XX.        .XXXXX.      ",
".XXX.  .XX.       .XXXXX.       ",
" .XXX..XXX.      .XXXXX.        ",
"  ..XXXXXXX.    .XXXXX.         ",
"    ..XXXXXX.  .XXXXX.          ",
"      ..XXXXX..XXXXX.           ",
"        .XXXXXXXXXX.            ",
"         .XXXXXXXX.             ",
"        .XXXXXXXXXX.            ",
"       .XXXXX..XXXXX.           ", 
"     ..XXXXX.  .XXXXX.          ", 
"    .XXXXXX.    .XXXXX.         ",
"   .XX..XXX.     .XXXXX.        ",
"  .XX.  .XX.      .XXXXX.       ",
" .XX.  .XXX.       .XXXXX.      ",
" .XX...XXX.         .XXXXX.     ",
" .XXXXXXX.           .XXXXX.    ",
" .XXXXXX.             .XXXX.    ",
"  ......               ....     ",
"                                ",
"                                ",
"                                ",
"                                ",
"                                ",
  "13,15"
};

  int i, row, col;
  Uint8 data[4*32];
  Uint8 mask[4*32];
  int hot_x, hot_y;

  i = -1;
  for ( row=0; row<32; ++row ) {
    for ( col=0; col<32; ++col ) {
      if ( col % 8 ) {
        data[i] <<= 1;
        mask[i] <<= 1;
      } else {
        ++i;
        data[i] = mask[i] = 0;
      }
      switch (image[4+row][31-col]) {
        case 'X':
          data[i] |= 0x01;
          mask[i] |= 0x01;
          break;
        case '.':
          mask[i] |= 0x01;
          break;
        case ' ':
          break;
      }
    }
  }
  sscanf(image[4+row], "%d,%d", &hot_x, &hot_y);
  pointer = SDL_GetCursor();
  scissors = SDL_CreateCursor(data, mask, 32, 32, hot_x, hot_y);
  SDL_SetCursor(pointer);
}

#define MPYTH(a,b,c) ((mousex-a)*(mousex-a) + (mousey-b)*(mousey-b) <= (c)*(c))
void Editor::check(double mousex, double mousey, int pixelx, int pixely, bool click, bool onSpace)
{
  unsigned int i;
  bool canRemove = false;
  bool wasRemoving = false; 

  if(setGalaxy)
  {
    if(click)
    {
      uni.galaxies.back().setVX(-(uni.galaxies.back().x-mousex)*2e6);
      uni.galaxies.back().setVY(-(uni.galaxies.back().y-mousey)*2e6);
      uni.calcStars();
      setGalaxy = false;
      canPut = true;
    }
  } 
  else 
  {
    if(!onSpace)
    {
      canPut = false;
    }
    else
    {
      canPut = true;
      for(i = 0; i < uni.holes.size(); i++)
      {
	if( MPYTH(uni.holes[i].x, uni.holes[i].y, uni.holes[i].radius) )
	{
	  canPut = false;

	  /* pruefen ob der ueberhaupt geloescht werden darf */
	  if(!uni.holes[i].level || all)
	  {
	    canRemove = true;

	    if(click)
	    {
	      if(!all)
	      {
	        massreserve += uni.holes[i].mass;
	      }
	      uni.holes.erase(uni.holes.begin()+i);
	      wasRemoving = true;
	    }
	  }
	}
      }

      if(!wasRemoving)
      {
	for(i = 0; i < uni.galaxies.size(); i++)
	{
	  if( MPYTH(uni.galaxies[i].x, uni.galaxies[i].y, uni.galaxies[i].radius*4) )
	  {
	    canPut = false;

	    if(all)
	    {
	      canRemove = true;
	      if(click)
	      {
		uni.galaxies.erase(uni.galaxies.begin()+i);
		uni.calcStars();
		wasRemoving = true;
	      }
	    }
	  }
	}
      }

      if(!wasRemoving)
      {
	if( MPYTH(uni.goal.x, uni.goal.y, uni.goal.radius) )
	{
	  canPut = false;

	  if(all)
	  {
	    canRemove = true;
	    if(click)
	    {
	      uni.goal = Goal();
	      wasRemoving = true;
	    }
	  }
	}
      }
    
      if(canRemove && !wasRemoving)
      {
        SDL_SetCursor(scissors);
      } else {
	SDL_SetCursor(pointer);
      }

      if(canPut)
      {
	if(click)
	{
	  if(!all)
	  {
	    if(massreserve >= getHoleWeight())
	    {
	      massreserve -= getHoleWeight();
	      uni.holes.push_back(Blackhole(mousex,mousey,getHoleWeight()));        
	      canPut = false;
	      canRemove = true;
              SDL_SetCursor(scissors);
	    }
	  }
	  else
	  {
	    switch(type)
	    {
	      case hole:
		uni.holes.push_back(Blackhole(mousex,mousey,getHoleWeight()));
		break;
	      case bulge:
		setGalaxy = true;
		canPut = false;
		galaxyX = pixelx;
		galaxyY = pixely;
		srand(time(NULL));
		uni.galaxies.push_back(Galaxy(mousex,mousey,getBulgeWeight(),(rand()%2),(rand()%2)));
		uni.calcStars();
		break;
	      case goal:
		uni.goal.setX(mousex);
		uni.goal.setY(mousey);
		uni.goal.setRadius(getGoalRadius());
		SDL_SetCursor(scissors);
		break;
	      default:
		// nix setzen
		break;
	    }
	  }
	}
      }
    }
  }
}

void Editor::drawMouse(SpaceDisplay* display)
{
  display->getDisplay()->OrthoMode();

  int mousex, mousey;
  SDL_GetMouseState(&mousex, &mousey);

  if(setGalaxy)
  {
    glColor3f(1,1,0);
    display->getIllustrator()->drawLine(galaxyX,galaxyY,mousex,mousey,2,true);
    glColor3f(1,1,1);
  }

  if(canPut && (all || (massreserve >= getHoleWeight())))
  {
    //display->getDisplay()->PerspectiveMode();
    glEnable(GL_BLEND);
    if(!all && massreserve < 2*getHoleWeight())
    {
      glColor3f(1,0,0.5);
    }

    switch(getType())
    {
      case Editor::hole:
	display->getPictureBook()->useTexture("hole");
	break;
      case Editor::bulge:
	display->getPictureBook()->useTexture("galaxy");
	break;
      case Editor::goal:
	display->getPictureBook()->useTexture("goal");
	break;
    }

    double size = getSize();
    if(display->getDisplay()->getHeight() > display->getDisplay()->getWidth())
    {
      size *= display->getDisplay()->getWidth();
    } else {
      size *= display->getDisplay()->getHeight();
    }
    display->getIllustrator()->putImage(mousex-size,mousey-size,size*2,size*2);

    glColor3f(1,1,1);
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

double Editor::getBulgeWeight()
{
  switch(size)
  {
    case small:
      return BULGESMALLMASS;
      break;
    case medium:
      return BULGEMEDIUMMASS;
      break;
    case large:
      return BULGELARGEMASS;
      break;
    default:
      return 0;
      break;
  }
}



double Editor::getGoalRadius()
{
  switch(size)
  {
    case small:
      return SMALLGOAL;
      break;
    case medium:
      return MEDIUMGOAL;
      break;
    case large:
      return LARGEGOAL;
      break;
    default:
      return 0;
      break;
  }
}

double Editor::getSize()
{
  switch(type)
  {
    case hole:
      return HOLESIZE*sqrt(getHoleWeight()/HOLEMEDIUMMASS);//*0.00000000002;
      // break;
    case bulge:
      return BULGESIZE*sqrt(getBulgeWeight()/1.7e10); //getBulgeWeight()*0.000000000165;
      // break;
    case goal:
      return getGoalRadius()*1.38;
      // break;
    default:
      break;
  }
  return 0.0;
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

  if(flags.checkFlag(ButtonFlags::putHole))
  {
    type = hole;
  }

  if(flags.checkFlag(ButtonFlags::putBulge))
  {
    type = bulge;
  }

  if(flags.checkFlag(ButtonFlags::putGoal))
  {
    type = goal;
  }
}

SpaceDisplay::BridgeView Editor::getView()
{
  return (all)?SpaceDisplay::EditorView:SpaceDisplay::PutView;
}
