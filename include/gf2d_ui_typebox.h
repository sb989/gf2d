#ifndef __GF2D_UI_TYPEBOX_H_
#define __GF2D_UI_TYPEBOX_H_
#include "gfc_list.h"
#include "gf2d_ui_box.h"
#include "gf2d_ui_button.h"
#include "gf2d_ui_textbox.h"
#include "gf2d_controls_state.h"
#include <string.h>
typedef struct
{
  Sprite * sprite;
  char * text;
  char * font_filename;
  int text_height;
  ButtonInfo *bi;
  int selected;
  int refcount;
  int buttRefCount;
  int char_limit;
  int char_count;
}TypeBox;

void gf2d_ui_typebox_init();
void gf2d_ui_typebox_load(SJson * typeboxes);
TypeBox * gf2d_ui_typebox_new();
TypeBox * gf2d_ui_typebox_init_typebox(int height,int width,int posx,int posy, char * font_filename);
void gf2d_ui_typebox_update();
void gf2d_ui_typebox_update_text(TypeBox * tb,char * text);
void gf2d_ui_typebox_clicked(void * self);
void gf2d_ui_typebox_draw(TypeBox *tb);
void gf2d_ui_typebox_draw_all();
void gf2d_ui_typebox_close();
void gf2d_ui_typebox_delete_text(TypeBox *tb);
TypeBox * gf2d_ui_typebox_get_typebox(int ind);
int gf2d_ui_typebox_get_count();
char * gf2d_ui_typebox_get_text(int ind);
void gf2d_ui_typebox_clear_list();


#endif
