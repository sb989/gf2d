#ifndef __GF2D_MAIN_GAME_H_
#define __GF2D_MAIN_GAME_H_
#include "gf2d_tilemap.h"
#include "gf2d_controls_state.h"
#include "gf2d_player.h"
#include "gf2d_pause_menu.h"
void gf2d_main_game_init();
void gf2d_main_game_draw();
void gf2d_main_game_close();
void gf2d_main_game_update();
void gf2d_main_game_set_box_color(Vector4D color);
void gf2d_main_game_set_paused(int p);
void gf2d_main_game_init_ui();
int gf2d_main_game_get_paused();


#endif
