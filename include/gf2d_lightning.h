#ifndef __GF2D_LIGHTNING_H_
#define __GF2D_LIGHTNING_H_
#include "gf2d_beam.h"
#include "chipmunk/chipmunk.h"

Beam * gf2d_lightning_init();
Beam * gf2d_lightning_new();
void gf2d_lightning_animate();
cpShapeFilter gf2d_lightning_filter();
void gf2d_lightning_close();

#endif
