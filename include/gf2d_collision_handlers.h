#ifndef __GF2D_COLLISION_HANDLERS_H_
#define __GF2D_COLLISION_HANDLERS_H_
#define ENTITY 0
#define BUTTON 1
#define MOUSE 2
#define PLAYER 3
#define OBJECTS 4
#define FIRE 5
#define LIGHTNING 6
#define ROCK 7
#define WIND 8
#define WATER 9
#define ICICLE 18
#define FIREBALL 19
#define ENEMIES 10

#include "chipmunk/chipmunk.h"
#include "simple_logger.h"
void gf2d_collision_handlers_add_all(cpSpace * space);
void gf2d_collision_handlers_mouse_button(cpSpace * space);
cpBool gf2d_collision_handlers_always_collide(cpArbiter *arb,cpSpace *space,void *data);
cpBool gf2d_collision_handlers_open_file(cpArbiter *arb,cpSpace *space,void *data);
cpBool gf2d_collision_handlers_test(cpArbiter *arb,cpSpace *space,void *data);
void gf2d_collision_handlers_open_file_done(cpArbiter * arb,cpSpace *space, void *data);
void gf2d_collision_handlers_player_enemy(cpSpace *space);
void gf2d_collision_handlers_fire_enemy(cpSpace *space);
void gf2d_collision_handlers_lightning_enemy(cpSpace *space);
cpBool gf2d_collision_handlers_push_back(cpArbiter *arb,cpSpace *space,void *data);
void gf2d_collision_handlers_set_white(cpArbiter * arb,cpSpace *space, void *data);

#endif
