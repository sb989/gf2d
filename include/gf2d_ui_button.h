#ifndef __GF2D_UI_BUTTON_
#define __GF2D_UI_BUTTON_
#include "../Chipmunk-7.0.3/include/chipmunk/chipmunk.h"
#include "gf2d_ui_box.h"
#include "gf2d_physics.h"
typedef struct
{
  BoxInfo * boxInfo;
  char * func_name;
  Sprite * s1;
  Sprite * s2;
  cpBody * body;
  cpShape * shape;
}ButtonInfo;


void gf2d_ui_button_init();
void gf2d_ui_button_load_buttons(SJson *buttons);
void gf2d_ui_button_switch_box_sprite(ButtonInfo * bi);
void gf2d_ui_button_draw_all();
void gf2d_ui_button_sprite_init(SJson * value,ButtonInfo * tbi);
ButtonInfo * gf2d_ui_button_new();
void gf2d_ui_button_close();
void gf2d_ui_button_setup_collision_body(ButtonInfo *b);

#endif
