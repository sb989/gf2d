#ifndef __GF2D_UI_TEXTBOX_H_
#define __GF2D_UI_TEXTBOX_H_
#include "gf2d_ui_box.h"
#include <string.h>
#include <SDL_ttf.h>
#include "gf2d_sprite.h"
typedef struct
{
  int height,width;
  Sprite *sprite;
  //BoxInfo * box;
  Vector2D pos;
  char * text;
}TextBox;

void gf2d_ui_textbox_init();
void gf2d_ui_textbox_load(SJson * textboxes);
TextBox * gf2d_ui_textbox_new();
TextBox *gf2d_ui_textbox_init_textbox(char * file_location,int height,int width,char * text,int posx,int posy);
void gf2d_ui_textbox_draw(TextBox * txt);
void gf2d_ui_textbox_close();
void gf2d_ui_textbox_draw_all();
int gf2d_ui_textbox_letter_pos(char c);
Sprite * gf2d_ui_textbox_load_sprite(char * text,char * file_location,int h);

#endif
