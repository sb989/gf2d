#include "gf2d_pause_menu.h"

void gf2d_pause_menu_init()
{
  gf2d_ui_load("json_files/pause_menu.json");
}

void gf2d_pause_menu_close()
{
  gf2d_ui_clear();
  gf2d_main_game_set_paused(0);
}


void gf2d_pause_menu_exit_game(void * nothing)
{
  gf2d_game_set_done(1);
}
