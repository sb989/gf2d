#include "gf2d_game_over.h"


void gf2d_game_over_init()
{
  gf2d_ui_load("json_files/game_over.json");
}

void gf2d_game_over_close()
{
  gf2d_ui_clear();
}
