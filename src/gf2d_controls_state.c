#include "gf2d_controls_state.h"
#include "gf2d_physics.h"

typedef struct
{
  Bool lMouseDown;
  Bool rMouseDown;
  Bool lMouseReleased;
  Bool rMouseReleased;
  Bool openFile;
  Bool mouseColliding;
  SDL_Event e;
  int clickTime,releaseTime;
  int mx,my;
  const Uint8 * keys;

}ControlsManager;

static ControlsManager cManager = {0};

void gf2d_controls_manager_init()
{
  cManager.lMouseDown = false;
  cManager.rMouseDown = false;
  cManager.lMouseReleased = false;
  cManager.rMouseReleased = false;
  cManager.openFile = false;
  cManager.mouseColliding = false;
  cManager.mx = 0;
  cManager.my = 0;
  cManager.clickTime = 0;
  cManager.releaseTime = 0;
}

void gf2d_controls_update()
{
  //slog("doing controls update");
  SDL_PumpEvents();   // update SDL's internal event structures
  cManager.keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
  SDL_PollEvent(&cManager.e);
  SDL_GetMouseState(&cManager.mx,&cManager.my);
  if(cManager.mouseColliding == true)
  {
    gf2d_update_mouse_state();
  }

}

void gf2d_set_mouse_colliding(Bool m)
{
  cManager.mouseColliding = m;
}

Vector2D gf2d_mouse_pos()
{
  return vector2d(cManager.mx,cManager.my);
}

SDL_Event gf2d_get_event()
{
  return cManager.e;
}

void gf2d_update_mouse_state()
{
  //slog("updating mouse state");
  if(cManager.e.button.button == SDL_BUTTON_LEFT)
  {
    if(cManager.e.type == SDL_MOUSEBUTTONDOWN)
    {
      //slog("left mouse is down");
      cManager.clickTime = SDL_GetTicks();
      cManager.lMouseDown = true;
    }
    else if(cManager.e.type==SDL_MOUSEBUTTONUP)
    {
      //slog("left mouse button is up");
      if(cManager.lMouseDown && !gf2d_left_released())
      {
        //slog("left is being released");
        gf2d_set_left_released(true);
        cManager.releaseTime = SDL_GetTicks();
        cManager.lMouseDown = false;
      }
      else if (cManager.lMouseDown && gf2d_left_released())
      {
        gf2d_set_left_released(false);
        cManager.lMouseDown = false;
      }

      else if(!cManager.lMouseDown)
        cManager.lMouseDown = false;
    }

  }
  else
  {
    cManager.lMouseDown = false;
    gf2d_set_left_released(false);
  }
  if(cManager.e.button.button ==SDL_BUTTON_RIGHT)
  {
    if(cManager.e.type == SDL_MOUSEBUTTONDOWN)
    {
      cManager.rMouseDown = true;
    }
    else if(cManager.e.type==SDL_MOUSEBUTTONUP)
    {
      if(cManager.rMouseDown)
        gf2d_set_right_released(true);
      else
        gf2d_set_right_released(false);
      cManager.rMouseDown = false;
    }
  }
}

Bool gf2d_left_mouse_down()
{
  //gf2d_update_mouse_state();
  return cManager.lMouseDown;
}

void gf2d_set_open_file(Bool b)
{
  cManager.openFile = b;
}

Bool gf2d_left_released()
{
  //gf2d_update_mouse_state();
  //int timeD = SDL_GetTicks()-cManager.releaseTime;
  //slog("time since left released was set is %d",timeD);
  //if(timeD>10)
    //return false;
  //else
  //{
    //slog("time since left released was set is %d",timeD);
    return cManager.lMouseReleased;
  //}
}

void gf2d_set_left_released(Bool b)
{
  //slog("set lMouseReleased to %d",b);
  cManager.lMouseReleased = b;
}

void gf2d_set_right_released(Bool b)
{
  cManager.rMouseReleased = b;
}

Bool gf2d_right_released()
{
  return cManager.rMouseReleased;
}

Bool gf2d_right_mouse_down()
{
  return cManager.rMouseDown;
}

Bool gf2d_key_pressed(SDL_Scancode scancode)
{
  return cManager.keys[scancode];
}

SDL_KeyboardEvent gf2d_get_keyboard_event()
{
  return cManager.e.key;
}

void gf2d_update_mouse_position(void *ent)
{
  Entity * e = (Entity *)ent;
  if(!cManager.openFile)
  {
    cpVect pos;
    //cpBB bb;
    SDL_GetMouseState(&cManager.mx,&cManager.my);
    e->position = gf2d_mouse_pos();
    pos = cpv(e->position.x,e->position.y);
    cpBodySetPosition(e->body,pos);
    //slog("angle is %f",vector2d_angle(gf2d_mouse_pos()));
    //cpSpaceReindexShapesForBody(cpBodyGetSpace(e->body),e->body);
    //bb = cpShapeGetBB(e->shape);
    //slog("button t is %f b is %f l is %f r is %f ",bb.t,bb.b,bb.l,bb.r);
  }
  else
  {
    return;
  }
}



cpShapeFilter gf2d_mouseFilter()
{
  cpGroup group;
  cpBitmask mask;
  cpBitmask cat;
  cpShapeFilter filter;
  group = 0;
  mask = BUTTON;
  cat = MOUSE;
  filter = cpShapeFilterNew(group,cat,mask);
  return filter;
}
