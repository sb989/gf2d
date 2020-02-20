#ifndef __GF2D_UI_BOX_
#define __GF2D_UI_BOX_
#include <string.h>
#include "simple_logger.h"
#include "simple_json.h"
#include "simple_json_array.h"
#include "gfc_list.h"
#include "gf2d_sprite.h"
#include "gf2d_graphics.h"
#include "gf2d_ui_helper_functions.h"
typedef struct
{
  Sprite * s;
  int xOffset;
  int yOffset;
  int width;
  int height;
  int resizex;
  int resizey;
  int posx;
  int posy;
  Vector4D color;
  Uint8 image_or_rect;
}BoxInfo;


void gf2d_ui_box_init();


void gf2d_ui_box_load_boxes(SJson *sprites);

SJson * gf2d_ui_box_get_sprite_info_from_json(SJson *json_file,char * key);

void gf2d_ui_box_draw_all();
void gf2d_ui_box_draw(BoxInfo *s);
BoxInfo * gf2d_ui_box_new();
BoxInfo * gf2d_ui_box_info_rect(SJson * value);
BoxInfo * gf2d_ui_box_info_image(SJson * value);
void gf2d_ui_box_clear_box_info(BoxInfo * si);
void gf2d_ui_box_close();



#endif
