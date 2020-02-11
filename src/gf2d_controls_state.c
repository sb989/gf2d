#include "gf2d_controls_state.h"


typedef struct
{
  Bool lMouseDown;
  Bool rMouseDown;
  SDL_Event e;
  int mx,my;
  const Uint8 * keys;
}ControlsManager;

ControlsManager cManager = {0};

void gf2d_controls_manager_init()
{
  cManager.lMouseDown = false;
  cManager.rMouseDown = false;
  cManager.mx = 0;
  cManager.my = 0;
}

void gf2d_controls_update()
{
  SDL_PumpEvents();   // update SDL's internal event structures
  cManager.keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
  SDL_PollEvent(&cManager.e);
  SDL_GetMouseState(&cManager.mx,&cManager.my);

  if(cManager.e.type == SDL_MOUSEBUTTONDOWN && cManager.e.button.button == SDL_BUTTON_LEFT)
  {
    cManager.lMouseDown = true;
  }
  if(cManager.e.type==SDL_MOUSEBUTTONUP)
  {
    cManager.lMouseDown = false;
    cManager.rMouseDown = false;
  }
  if(cManager.e.type == SDL_MOUSEBUTTONDOWN && cManager.e.button.button ==SDL_BUTTON_RIGHT)
  {
    cManager.rMouseDown = true;
  }
}

Vector2D gf2d_mouse_pos()
{
  return vector2d(cManager.mx,cManager.my);
}

Bool gf2d_left_mouse_down()
{
  return cManager.lMouseDown;
}

Bool gf2d_right_mouse_down()
{
  return cManager.rMouseDown;
}

Bool gf2d_key_pressed(SDL_Scancode scancode)
{
  return cManager.keys[scancode];
}
