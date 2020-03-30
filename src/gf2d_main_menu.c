#include "gf2d_main_menu.h"

void gf2d_main_menu_init()
{
  //gf2d_ui_init();
  gf2d_ui_load("json_files/main_menu.json");
}

void gf2d_main_menu_close()
{
  gf2d_ui_clear();
}
