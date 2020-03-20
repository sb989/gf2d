#include "gf2d_collision_handlers.h"
#include "gfc_vector.h"
#include "gf2d_ui_button.h"
#include "gf2d_controls_state.h"
#include "gf2d_gtk.h"
#include <time.h>
void gf2d_collision_handlers_add_all(cpSpace * space)
{
  gf2d_collision_handlers_mouse_button(space);
}


void gf2d_collision_handlers_mouse_button(cpSpace * space)
{
  cpCollisionHandler *handler;
  handler = cpSpaceAddCollisionHandler(space,MOUSE,BUTTON);
  //handler->beginFunc = (cpCollisionBeginFunc)gf2d_collision_handlers_test;
  handler->preSolveFunc = (cpCollisionPreSolveFunc)gf2d_collision_handlers_open_file;
  //handler->postSolveFunc = (cpCollisionPostSolveFunc)gf2d_collision_handlers_open_file;
  handler->separateFunc = (cpCollisionSeparateFunc)gf2d_collision_handlers_turn_red;
}


cpBool gf2d_collision_handlers_open_file(cpArbiter *arb,cpSpace *space,void *data)
{
  cpBody *a,*b;
  ButtonInfo *butt;
  double time;

  /*Vector4D blue;
  blue.x =0;
  blue.y = 0;
  blue.z = 255;
  blue.w = 255;*/
  cpArbiterGetBodies(arb,&a,&b);
  butt = (ButtonInfo*)cpBodyGetUserData(b);
  //butt->boxInfo->color = blue;
  //slog("working");

  //slog("in chipmunk loop");
  gf2d_set_mouse_colliding(true);
  clock_t begin = clock();
  gf2d_controls_update();
  clock_t end = clock();
  time = (double)(end-begin)/CLOCKS_PER_SEC;


  //slog("done updating controls");
  //slog("gf2d_left_mouse_down is %d",gf2d_left_mouse_down());
  //slog("time is %d",SDL_GetTicks());
  if(gf2d_left_mouse_down() && butt->onHold)
  {
    slog("gf2d_controls_update took %f to execute",time);
    begin = clock();
    butt->onHold(butt);
    end = clock();
    time = (double)(end-begin)/CLOCKS_PER_SEC;
    slog("onHold took %f to execute",time);
  }
  else if(gf2d_left_released() && butt->onRelease)
  {
    //slog("poop");
    butt->onRelease(butt);
    //slog("left released");
    //gf2d_set_left_released(false);
  }



  return 0;

}

void gf2d_collision_handlers_turn_red(cpArbiter * arb,cpSpace *space, void *data)
{
  //slog("Red");
  gf2d_set_mouse_colliding(false);

  /*cpBody *a,*b;
  Vector4D red;
  ButtonInfo *butt;
  red.x =255;
  red.y = 0;
  red.z = 0;
  red.w = 255;
  cpArbiterGetBodies(arb,&a,&b);
  butt = (ButtonInfo*)cpBodyGetUserData(b);*/
  //butt->boxInfo->color = red;
}
