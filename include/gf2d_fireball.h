#ifndef __GF2D_FIREBALL_H_
#define __GF2D_FIREBALL_H_
#include "gf2d_projectile.h"
#include "chipmunk/chipmunk.h"

Projectile * gf2d_fireball_init(int player);
Projectile * gf2d_fireball_new(int player);
Projectile * gf2d_fireball_shoot(int player);
void gf2d_fireball_animate();
void gf2d_fireball_close();
void gf2d_fireball_lower_count(void * p);
cpShapeFilter gf2d_fireball_filter();



#endif
