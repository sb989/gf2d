#include "gf2d_collision_handlers.h"
#include <time.h>
#include "gfc_vector.h"
#include "gf2d_ui_button.h"
#include "gf2d_controls_state.h"
#include "gf2d_gtk.h"
void gf2d_collision_handlers_add_all(cpSpace * space)
{
  gf2d_collision_handlers_mouse_button(space);
  gf2d_collision_handlers_player_enemy(space);
}

void gf2d_collision_handlers_player_enemy(cpSpace *space)
{
  cpCollisionHandler *handler;
  handler = cpSpaceAddCollisionHandler(space,PLAYER,ENEMIES);
  handler->preSolveFunc = (cpCollisionPreSolveFunc)gf2d_collision_handlers_push_back;
}



void gf2d_collision_handlers_mouse_button(cpSpace * space)
{
  cpCollisionHandler *handler;
  handler = cpSpaceAddCollisionHandler(space,MOUSE,BUTTON);
  //handler->beginFunc = (cpCollisionBeginFunc)gf2d_collision_handlers_test;
  handler->preSolveFunc = (cpCollisionPreSolveFunc)gf2d_collision_handlers_open_file;
  //handler->postSolveFunc = (cpCollisionPostSolveFunc)gf2d_collision_handlers_open_file;
  handler->separateFunc = (cpCollisionSeparateFunc)gf2d_collision_handlers_open_file_done;
}

cpBool gf2d_collision_handlers_push_back(cpArbiter *arb,cpSpace *space,void *data)
{
  cpBody *body_a,*body_b;
  cpShape *shape_a,*shape_b;
  cpCollisionType a,b;
  //Player *p;
  Entity *entA,*entB;
  slog("collision");
  cpArbiterGetBodies(arb,&body_a,&body_b);
  entA = (Entity*)cpBodyGetUserData(body_a);
  entB = (Entity*)cpBodyGetUserData(body_b);
  if(entA->_inuse == 0 || entB->_inuse ==0)
    return false;
  cpArbiterGetShapes(arb,&shape_a,&shape_b);
  a = cpShapeGetCollisionType(shape_a);
  b = cpShapeGetCollisionType(shape_b);
  if(a==PLAYER && b==ENEMIES)
  {

    slog("player and enemy colliding");
    gf2d_main_game_set_box_color(vector4d(0,0,0,255));
  }

  return 0;
}

void gf2d_collision_handlers_set_white(cpArbiter * arb,cpSpace *space, void *data)
{
  gf2d_main_game_set_box_color(vector4d(255,255,255,255));
}

cpBool gf2d_collision_handlers_open_file(cpArbiter *arb,cpSpace *space,void *data)
{
  cpBody *a,*b;
  ButtonInfo *butt;

  cpArbiterGetBodies(arb,&a,&b);
  butt = (ButtonInfo*)cpBodyGetUserData(b);

  gf2d_set_mouse_colliding(true);

  gf2d_controls_update();

  if(gf2d_left_mouse_down() && butt->onHold)
  {
    butt->onHold(butt);
  }
  else if(gf2d_left_released() && butt->onRelease)
  {
    butt->onRelease(butt);
    gf2d_set_left_released(false);
  }

  return 0;
}

void gf2d_collision_handlers_open_file_done(cpArbiter * arb,cpSpace *space, void *data)
{
  gf2d_set_mouse_colliding(false);

}
