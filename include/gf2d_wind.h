#ifndef __GF2D_WIND_H_
#define __GF2D_WIND_H_

#include "gf2d_beam.h"
#include "chipmunk/chipmunk.h"

Beam * gf2d_wind_init();
Beam * gf2d_wind_new();
cpShapeFilter gf2d_wind_filter();
void gf2d_wind_close();


#endif
