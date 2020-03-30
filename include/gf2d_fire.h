#ifndef __GF2D_FIRE_H_
#define __GF2D_FIRE_H_
#include "gf2d_beam.h"
#include "chipmunk/chipmunk.h"

Beam * gf2d_fire_init();
Beam * gf2d_fire_new();
void gf2d_fire_animate();
cpShapeFilter gf2d_fire_filter();
void gf2d_fire_close();


#endif
