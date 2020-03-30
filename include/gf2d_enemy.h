#ifndef __GF2D_ENEMY_H_
#define __GF2D_ENEMY_H_
#include "gfc_vector.h"
#include "chipmunk/chipmunk.h"
#include "gfc_list.h"
#include "gf2d_entity.h"
#include <string.h>

void gf2d_enemy_init();
void gf2d_enemy_init_enemy(Vector2D pos,char * color);
cpShapeFilter gf2d_enemy_filter();
void gf2d_enemy_close();
void gf2d_enemy_draw_bb_all();
void gf2d_enemy_draw_bb(Entity *e);


#endif
