#ifndef __GF2D_ROCK_H_
#define __GF2D_ROCK_H_
#include "gf2d_projectile.h"
#include "chipmunk/chipmunk.h"

Projectile * gf2d_rock_init(int player);
Projectile * gf2d_rock_new(int player);
Projectile * gf2d_rock_shoot(int player);
void gf2d_rock_animate();
void gf2d_rock_close();
cpShapeFilter gf2d_rock_filter();
void gf2d_rock_lower_count(void * p);

#endif
