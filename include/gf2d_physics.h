#ifndef __GF2D_PHYSICS_H_
#define __GF2D_PHYSICS_H_
#include "gf2d_collision_handlers.h"
#include "../Chipmunk-7.0.3/include/chipmunk/chipmunk.h"
//#include "ChipmunkDemo.h"
//#include "sokol.h"
//#include "chipmunk/chipmunk_private.h"
//#include "ChipmunkDemoTextSupport.h"
//#include "chipmunk/chipmunk.h"
typedef struct
{
  cpSpace *space;
}PhysicsSpace;

void gf2d_physics_init();
cpShape * gf2d_physics_add_square_body(cpFloat length,cpFloat width,cpFloat radius,uint8_t type);
void gf2d_physis_add_circle_body();
void gf2d_physics_close();
void gf2d_physics_update();
cpSpace * gf2d_physics_get_space();

#endif
