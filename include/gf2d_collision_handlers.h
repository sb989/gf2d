#ifndef __GF2D_COLLISION_HANDLERS_H_
#define __GF2D_COLLISION_HANDLERS_H_
#define MOUSE 2
#define ENTITY 0
#define BUTTON 1
#include "chipmunk/chipmunk.h"
#include "simple_logger.h"
void gf2d_collision_handlers_add_all(cpSpace * space);
void gf2d_collision_handlers_mouse_button(cpSpace * space);
cpBool gf2d_collision_handlers_always_collide(cpArbiter *arb,cpSpace *space,void *data);
cpBool gf2d_collision_handlers_open_file(cpArbiter *arb,cpSpace *space,void *data);
cpBool gf2d_collision_handlers_test(cpArbiter *arb,cpSpace *space,void *data);
void gf2d_collision_handlers_turn_red(cpArbiter * arb,cpSpace *space, void *data);



#endif
