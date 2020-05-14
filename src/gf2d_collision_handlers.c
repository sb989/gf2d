#include "gf2d_collision_handlers.h"
#include <time.h>
#include "gfc_vector.h"
#include "gf2d_ui_button.h"
#include "gf2d_controls_state.h"
#include "gf2d_gtk.h"
#include "gf2d_projectile.h"
#include "gf2d_enemy.h"
void gf2d_collision_handlers_add_all(cpSpace * space)
{
  gf2d_collision_handlers_mouse_button(space);
  gf2d_collision_handlers_player_enemy(space);
  gf2d_collision_handlers_rock_enemy(space);
  gf2d_collision_handlers_icicle_enemy(space);
  gf2d_collision_handlers_fireball_enemy(space);
  gf2d_collision_handlers_player_coin(space);

  gf2d_collision_handlers_environment_coin(space);
  gf2d_collision_handlers_environment_rock(space);
  gf2d_collision_handlers_environment_icicle(space);
  gf2d_collision_handlers_environment_fireball(space);
  gf2d_collision_handlers_environment_enemy(space);
  gf2d_collision_handlers_environment_player(space);
}

void gf2d_collision_handlers_player_enemy(cpSpace *space)
{
  cpCollisionHandler *handler;
  handler = cpSpaceAddCollisionHandler(space,PLAYER,ENEMIES);
  handler->preSolveFunc = (cpCollisionPreSolveFunc)gf2d_collision_handlers_push_back;
  handler->separateFunc = (cpCollisionSeparateFunc)gf2d_collision_handlers_set_white;
}

void gf2d_collision_handlers_rock_enemy(cpSpace *space)
{
  cpCollisionHandler *handler;
  handler = cpSpaceAddCollisionHandler(space,ROCK,ENEMIES);
  handler->preSolveFunc = (cpCollisionPreSolveFunc)gf2d_collision_handlers_proj_hit;
  //handler->separateFunc = (cpCollisionSeparateFunc)gf2d_collision_handlers_set_white;
}

void gf2d_collision_handlers_icicle_enemy(cpSpace *space)
{
  cpCollisionHandler *handler;
  handler = cpSpaceAddCollisionHandler(space,ICICLE,ENEMIES);
  handler->preSolveFunc = (cpCollisionPreSolveFunc)gf2d_collision_handlers_proj_hit;
  //handler->separateFunc = (cpCollisionSeparateFunc)gf2d_collision_handlers_set_white;
}

void gf2d_collision_handlers_fireball_enemy(cpSpace *space)
{
  cpCollisionHandler *handler;
  handler = cpSpaceAddCollisionHandler(space,FIREBALL,ENEMIES);
  handler->preSolveFunc = (cpCollisionPreSolveFunc)gf2d_collision_handlers_proj_hit;
  //handler->separateFunc = (cpCollisionSeparateFunc)gf2d_collision_handlers_set_white;
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

void gf2d_collision_handlers_player_coin(cpSpace *space)
{
  cpCollisionHandler *handler;
  handler = cpSpaceAddCollisionHandler(space,PLAYER,COIN);
  handler->preSolveFunc = (cpCollisionPreSolveFunc)gf2d_collision_handlers_pickup_coin;
}

void gf2d_collision_handlers_environment_coin(cpSpace *space)
{
  cpCollisionHandler *handler;
  handler = cpSpaceAddCollisionHandler(space,OBJECTS,COIN);
  handler->preSolveFunc = (cpCollisionPreSolveFunc)gf2d_collision_handlers_environment_collision;
}

void gf2d_collision_handlers_environment_rock(cpSpace *space)
{
  cpCollisionHandler *handler;
  handler = cpSpaceAddCollisionHandler(space,OBJECTS,ROCK);
  handler->preSolveFunc = (cpCollisionPreSolveFunc)gf2d_collision_handlers_environment_collision;
}

void gf2d_collision_handlers_environment_icicle(cpSpace *space)
{
  cpCollisionHandler *handler;
  handler = cpSpaceAddCollisionHandler(space,OBJECTS,ICICLE);
  handler->preSolveFunc = (cpCollisionPreSolveFunc)gf2d_collision_handlers_environment_collision;
}

void gf2d_collision_handlers_environment_fireball(cpSpace *space)
{
  cpCollisionHandler *handler;
  handler = cpSpaceAddCollisionHandler(space,OBJECTS,FIREBALL);
  handler->preSolveFunc = (cpCollisionPreSolveFunc)gf2d_collision_handlers_environment_collision;
}

void gf2d_collision_handlers_environment_player(cpSpace *space)
{
  cpCollisionHandler *handler;
  handler = cpSpaceAddCollisionHandler(space,OBJECTS,PLAYER);
  handler->preSolveFunc = (cpCollisionPreSolveFunc)gf2d_collision_handlers_environment_collision;
}
void gf2d_collision_handlers_environment_enemy(cpSpace *space)
{
  cpCollisionHandler *handler;
  handler = cpSpaceAddCollisionHandler(space,OBJECTS,ENEMIES);
  handler->preSolveFunc = (cpCollisionPreSolveFunc)gf2d_collision_handlers_environment_collision;
}

cpBool gf2d_collision_handlers_pickup_coin(cpArbiter *arb,cpSpace *space, void *data)
{
  cpBody *body_a,*body_b;
  //Player *p;
  Entity *entB;
  //slog("collision");
  cpArbiterGetBodies(arb,&body_a,&body_b);

  entB = (Entity*)cpBodyGetUserData(body_b);
  entB->_inuse = 0;
  return true;
}

cpBool gf2d_collision_handlers_proj_hit(cpArbiter *arb,cpSpace *space, void *data)
{
  cpBody *body_a,*body_b;
  cpShape *shape_a,*shape_b;
  cpCollisionType a,b;
  //Player *p;
  Entity *entA,*entB;
  //slog("collision");
  cpArbiterGetBodies(arb,&body_a,&body_b);
  entA = (Entity*)cpBodyGetUserData(body_a);
  entB = (Entity*)cpBodyGetUserData(body_b);
  if(entA->_inuse == 0 || entB->_inuse ==0)
    return false;
  cpArbiterGetShapes(arb,&shape_a,&shape_b);
  a = cpShapeGetCollisionType(shape_a);
  b = cpShapeGetCollisionType(shape_b);
  if((a == ROCK || a == ICICLE || a == FIREBALL)&& (b == ENEMIES || b == OBJECTS))
  {
    Projectile * proj = gf2d_projectile_get_proj_from_ent(entA);
    if(proj->destroy == 1)
      return false;
    gf2d_projectile_destroy(proj);
    gf2d_enemy_take_damage(5,entB);
  }
  cpSpaceAddPostStepCallback(gf2d_physics_get_space(), (cpPostStepFunc)gf2d_enemy_drop_coin,entB, NULL);
  return true;
}

cpBool gf2d_collision_handlers_push_back(cpArbiter *arb,cpSpace *space,void *data)
{
  cpBody *body_a,*body_b;
  cpShape *shape_a,*shape_b;
  cpCollisionType a,b;
  //Player *p;
  Entity *entA,*entB;
  //slog("collision");
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

    cpVect velocity;
    cpVect zero = {0,0};
    if(gf2d_player_get_player(0)->invincible > 0)
      return false;
    if(cpveql(cpBodyGetVelocity(body_a),zero))
    {
      velocity = cpvnormalize(cpBodyGetVelocity(body_b));
      velocity = cpvmult(velocity,400);
    }
    else if(cpveql(cpBodyGetVelocity(body_b),zero))
    {
      if(gf2d_player_get_player(0)->ent->colliding == 1)
        return false;
      velocity = cpvnormalize(cpBodyGetVelocity(body_a));
      velocity = cpvneg(velocity);
      velocity = cpvmult(velocity,400);
    }
    else if(!cpveql(cpBodyGetVelocity(body_a),zero) && !cpveql(cpBodyGetVelocity(body_b),zero))
    {
      velocity = cpvnormalize(cpvsub(cpBodyGetVelocity(body_a),cpBodyGetVelocity(body_b)));
      velocity = cpvneg(velocity);
      velocity = cpvmult(velocity,400);
    }

    gf2d_player_get_player(0)->invincible = 1;
    gf2d_player_get_player(0)->ent->colliding = 1;
    cpBodySetVelocity(gf2d_player_get_player(0)->ent->body,velocity);
    gf2d_player_take_damage(1);
  }

  return 0;
}

cpBool gf2d_collision_handlers_environment_collision(cpArbiter *arb,cpSpace *space,void *data)
{
  cpBody *body_a,*body_b;
  cpShape *shape_a,*shape_b;
  cpCollisionType a,b;
  //Player *p;
  Entity *entB;
  //slog("collision");
  cpArbiterGetBodies(arb,&body_a,&body_b);
  entB = (Entity*)cpBodyGetUserData(body_b);
  if(entB->_inuse ==0)
    return false;
  cpArbiterGetShapes(arb,&shape_a,&shape_b);
  a = cpShapeGetCollisionType(shape_a);
  b = cpShapeGetCollisionType(shape_b);

  if(a==OBJECTS)
  {

    cpVect velocity;
    cpVect pos;
    slog("push backk");
    cpFloat factor = cpvlength(cpBodyGetVelocity(body_b));
    factor = factor/60.0f;
    pos = cpvnormalize(cpBodyGetVelocity(body_b));
    pos = cpvneg(pos);
    pos = cpvmult(pos,factor);
    pos = cpvadd(pos,cpBodyGetPosition(body_b));
    cpBodySetPosition(body_b,pos);
    entB->colliding = 1;
    velocity = cpv(0.f,0.f);
    cpBodySetVelocity(body_b,velocity);
  }

  return 0;
}

void gf2d_collision_handlers_set_white(cpArbiter * arb,cpSpace *space, void *data)
{
  //gf2d_main_game_set_box_color(vector4d(255,255,255,255));


}

cpBool gf2d_collision_handlers_open_file(cpArbiter *arb,cpSpace *space,void *data)
{
  cpBody *a,*b;
  ButtonInfo *butt;

  cpArbiterGetBodies(arb,&a,&b);
  butt = (ButtonInfo*)cpBodyGetUserData(b);

  gf2d_set_mouse_colliding(true);

  gf2d_controls_update();

  //slog("colliding");
  if(gf2d_left_mouse_down() && butt->onHold)
  {
    butt->onHold(butt);
  }
  else if(gf2d_left_released() && butt->onRelease)
  {
    //slog("calling thing");
    butt->onRelease(butt);
    gf2d_set_left_released(false);
  }

  if(gf2d_right_mouse_down() && butt->rightOnHold)
  {
    butt->rightOnHold(butt);
  }
  else if(gf2d_right_released() && butt->rightOnRelease)
  {
    butt->rightOnRelease(butt);
    gf2d_set_right_released(false);
  }

  return 0;
}

void gf2d_collision_handlers_open_file_done(cpArbiter * arb,cpSpace *space, void *data)
{
  gf2d_set_mouse_colliding(false);

}
