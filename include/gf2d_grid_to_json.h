#ifndef __GF2D_GRID_TO_JSON_H_
#define __GF2D_GRID_TO_JSON_H_
#include "gf2d_grid.h"
#include "gf2d_level_editor.h"
#include "../Chipmunk-7.0.3/include/chipmunk/chipmunk.h"
void gf2d_grid_to_json_create_json_grids(List * grids);
SJson * gf2d_grid_to_json_create_json_cell(ButtonInfo * cell,int posx,int posy);



#endif
