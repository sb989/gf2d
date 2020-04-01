#ifndef __GF2D_WATER_H_
#define __GF2D_WATER_H_
#include "gf2d_beam.h"
#include "chipmunk/chipmunk.h"

Beam * gf2d_water_init();
Beam * gf2d_water_new();
void gf2d_water_animate();
cpShapeFilter gf2d_water_filter();
void gf2d_water_close();

#endif
