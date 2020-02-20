#include "gf2d_ui.h"


void gf2d_ui_init()
{
  gf2d_ui_box_init();
  gf2d_ui_button_init();
}

void gf2d_ui_draw()
{
  gf2d_ui_button_draw_all();
}

void gf2d_ui_load(char * file_location)
{
  SJson *level,*boxes,*buttons;
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
}
