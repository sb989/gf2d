#ifndef __GF2D_UI_BUTTON_
#define __GF2D_UI_BUTTON_
#include "../Chipmunk-7.0.3/include/chipmunk/chipmunk.h"
#include "gf2d_ui_box.h"
#include "gf2d_physics.h"
#include "gf2d_find_functions.h"
typedef struct
{
  BoxInfo * boxInfo;
  char *onReleaseName;
  char *onHoldName;
  char *rightOnReleaseName;
  char *rightOnHoldName;

  void (*onRelease)(void*);
  void (*onHold)(void*);
  void (*rightOnRelease)(void*);
  void (*rightOnHold)(void*);

  void *onRelease_data;
  void *onHold_data;
  void *rightOnRelease_data;
  void *rightOnHold_data;

  Sprite * s1;
  Sprite * s2;
  cpBody * body;
  cpShape * shape;
  int refcount;
  int holdernum;
}ButtonInfo;


void gf2d_ui_button_init();
void gf2d_ui_button_load_buttons(SJson *buttons);
void gf2d_ui_button_switch_box_sprite(ButtonInfo * bi);
void gf2d_ui_button_draw_all();
void gf2d_ui_button_sprite_init(SJson * value,ButtonInfo * tbi);
ButtonInfo * gf2d_ui_button_new();
void gf2d_ui_button_close();
void gf2d_ui_button_setup_collision_body(ButtonInfo *b);
cpShapeFilter gf2d_ui_button_filter();
void gf2d_ui_touch(cpShape *a);
void gf2d_ui_button_set_func_name(SJson * value,ButtonInfo *tbi);
void gf2d_ui_button_update_collision_body(ButtonInfo *b);
void gf2d_ui_button_update_graphics_position(ButtonInfo *b);
void gf2d_ui_button_update_graphics_position_all();
void gf2d_ui_button_free_body(ButtonInfo * butt);
void gf2d_ui_button_clear_list();

#endif
