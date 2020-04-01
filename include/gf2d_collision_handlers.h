#ifndef __GF2D_COLLISION_HANDLERS_H_
#define __GF2D_COLLISION_HANDLERS_H_

#define ENTITY 0
#define BUTTON (1)
#define MOUSE (1<<1)
#define PLAYER (1<<2)
#define OBJECTS (1<<3)
#define FIRE (1<<4)
#define LIGHTNING (1<<5)
#define ROCK (1<<6)
#define WIND (1<<7)
#define WATER (1<<8)
#define ICICLE (1<<9)
#define FIREBALL (1<<10)
#define ENEMIES (1<<11)
#define BEAM (1<<12)
#define PROJECTILES (1<<13)


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
cpBool gf2d_collision_handlers_rock_hit(cpArbiter *arb,cpSpace *space, void *data);
void gf2d_collision_handlers_rock_enemy(cpSpace *space);

#endif
