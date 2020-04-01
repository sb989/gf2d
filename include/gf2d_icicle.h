#ifndef __GF2D_ICICLE_H_
#define __GF2D_ICICLE_H_
#include "gf2d_projectile.h"
#include "chipmunk/chipmunk.h"

Projectile * gf2d_icicle_init(int player);
Projectile * gf2d_icicle_new(int player);
Projectile * gf2d_icicle_shoot(int player);
void gf2d_icicle_animate();
void gf2d_icicle_close();
void gf2d_icicle_lower_count(void * p);
cpShapeFilter gf2d_icicle_filter();

#endif
