#ifndef __GF2D_GRID_H_
#define __GF2D_GRID_H_
#include "gf2d_ui_button.h"
#include "gf2d_level_editor.h"
#include "gf2d_draw.h"
typedef struct
{
  int rows,columns;
  List *cells;//are buttons;
  int height,width,posx,posy,xspacing,yspacing;
  int gridnum;
  int isEmpty;
  ButtonInfo *left,*right;
}Grid;

void gf2d_grid_init();
Grid * gf2d_grid_new_grid();
Grid * gf2d_grid_init_grid(int rows,int columns,int height,int width,int posx,int posy);
ButtonInfo * gf2d_grid_init_cell(int posx,int posy,int width,int height);
void gf2d_grid_set_sprite(void * cell);
void gf2d_grid_clear_cell(void *cell);
void gf2d_grid_clear();
void gf2d_grid_draw();
void gf2d_grid_draw_buttons(Grid *g);
void gf2d_grid_draw_grid(Grid *g);
void gf2d_grid_draw();
void gf2d_grid_left_button();
void gf2d_grid_right_clicked(void * nothing);
void gf2d_grid_left_clicked(void*nothing);
int gf2d_grid_get_left_clicked();
int gf2d_grid_get_right_clicked();
void gf2d_grid_right_button();
List * gf2d_grid_return_all();
void gf2d_grid_set_render_zero(Grid *g);
void gf2d_grid_set_render_one(Grid *g);
void gf2d_grid_clear_list();
void gf2d_grid_close();
#endif
