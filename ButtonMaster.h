#ifndef _BUTTONMASTER_H_
#define _BUTTONMASTER_H_

#include <GL/gl.h>
#include <SDL.h>
#include <iostream>
#include <math.h>

#include "PictureBook.h"
#include "Illustrator.h"
#include "GLdisplay.h"

class AbstractButtonFlags
{
  public:
  /* muss von Kindklassen gefuellt werden */
  enum Actions {
  };

  private:
  int flags;
  
  public:
  void activateFlag(Actions action);
  bool checkFlag(int action); /* statt int waer enum besser, aber geht nicht da kein Enum-Prototyping - loescht auch das Flag */
  bool viewFlag(int action); /* loescht nicht! */
  AbstractButtonFlags()
    : flags(0)
  {
  }
  virtual ~AbstractButtonFlags() {};
};

#include "ButtonFlags.h"

class Button
{
  public:
   std::string texture;
   float x;
   float y;
   float r;
   int active;
   ButtonFlags::Actions action;
   Button() : texture(""), x(0), y(0), r(0), active(0), action((ButtonFlags::Actions)4096) {}
};

class ButtonMaster
{
  private:
    #define MAX_BUTTONS 10
    Button buttons[MAX_BUTTONS];
    int buttonSize;
    PictureBook &textures;
    Illustrator &illustrator;

   public:
    void addButton(std::string texture, float x, float y, float r, ButtonFlags::Actions action);
    void clearButtons();
    void drawButtons();
    void checkButtons(ButtonFlags &flags, int x, int y);

    ButtonMaster(PictureBook &t, Illustrator &i);
};

#endif
