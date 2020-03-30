#include "gf2d_button_functions.h"



void gf2d_button_functions_init()
{
  gf2d_find_functions_init();
  gf2d_find_functions_add_func("gf2d_file_directory_open",&gf2d_file_directory_open);
  gf2d_find_functions_add_func("gf2d_ui_typebox_clicked",&gf2d_ui_typebox_clicked);
  gf2d_find_functions_add_func("gf2d_level_editor_load_sprite",&gf2d_level_editor_load_sprite);
  gf2d_find_functions_add_func("gf2d_file_directory_save",&gf2d_file_directory_save);
  gf2d_find_functions_add_func("gf2d_game_state_set_main_menu",&gf2d_game_state_set_main_menu);
  gf2d_find_functions_add_func("gf2d_game_state_set_level_editor",&gf2d_game_state_set_level_editor);
  gf2d_find_functions_add_func("gf2d_game_state_set_game",&gf2d_game_state_set_game);
}
