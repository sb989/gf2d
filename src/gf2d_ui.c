#include "gf2d_ui.h"

void gf2d_ui_init()
{
  slog("ui init func");
  gf2d_ui_box_init();
  gf2d_ui_button_init();
  gf2d_ui_textbox_init();
  gf2d_ui_typebox_init();
}

void gf2d_ui_update()
{
  if(gf2d_ui_typebox_get_count()>0)
    gf2d_ui_typebox_update();
  //gf2d_ui_button_update_graphics_position_all();
  gf2d_ui_draw();
}

void gf2d_ui_draw()
{
  gf2d_ui_button_draw_all();
  gf2d_ui_textbox_draw_all();
  gf2d_ui_typebox_draw_all();
}

void gf2d_ui_load(char * file_location)
{
  SJson *level,*boxes,*buttons,*textboxes,*typeboxes;
  level = sj_load(file_location);
  if(!level)
  {
    slog("failed to load json file");
    return;
  }
  boxes = sj_object_new();
  boxes = sj_object_get_value(level,"boxes");
  gf2d_ui_box_load_boxes(boxes);
  buttons = sj_object_new();
  buttons = sj_object_get_value(level,"buttons");
  gf2d_ui_button_load_buttons(buttons);
  textboxes = sj_object_new();
  textboxes = sj_object_get_value(level,"textboxes");
  gf2d_ui_textbox_load(textboxes);
  typeboxes = sj_object_new();
  typeboxes = sj_object_get_value(level,"typeboxes");
  gf2d_ui_typebox_load(typeboxes);
}

void gf2d_ui_clear()
{
  gf2d_ui_textbox_clear_list();
  gf2d_ui_typebox_clear_list();
  gf2d_ui_button_clear_list();
  gf2d_ui_box_clear_list();
}
