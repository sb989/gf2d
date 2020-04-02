#ifndef __GF2D_GAME_STATE_H_
#define __GF2D_GAME_STATE_H_
#include "gf2d_main_menu.h"
#include "gf2d_level_editor.h"
#include "gf2d_main_game.h"
#include "gf2d_game_over.h"
void gf2d_game_state_init();
void gf2d_game_state_update();
int gf2d_game_state_get_state();
int gf2d_game_state_get_old_state();
void gf2d_game_state_set_main_menu(void * nothing);
void gf2d_game_state_set_level_editor(void * nothing);
void gf2d_game_state_set_game(void * nothing);
int gf2d_game_state_get_update();


#endif
