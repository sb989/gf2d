#ifndef __GF2D_LEVEL_EDITOR_H_
#define __GF2D_LEVEL_EDITOR_H_
#include "gf2d_ui.h"
#include "gf2d_image_carousel.h"
#include "gf2d_grid.h"
#include "gf2d_grid_to_json.h"
void gf2d_level_editor_init();
void gf2d_level_editor_load_sprite(void * nothing);
void gf2d_level_editor_update();
Sprite * gf2d_level_editor_get_selected_sprite();
int gf2d_level_editor_get_selected_sprite_num();
char * gf2d_level_editor_get_save_file();
void gf2d_level_editor_close();
void gf2d_level_editor_set_save_new_file(char * filename);
void gf2d_level_editor_load();


#endif
