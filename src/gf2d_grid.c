#include "gf2d_grid.h"

static List * grids = NULL;

void gf2d_grid_init()
{
  grids = gfc_list_new();
  atexit(gf2d_grid_clear);
}

Grid * gf2d_grid_new_grid()
{
  Grid * temp = (Grid*)malloc(sizeof(Grid));
  temp->cells = gfc_list_new();
  grids = gfc_list_append(grids,temp);
  return temp;
}

Grid * gf2d_grid_init_grid(int rows,int columns,int height,int width,int posx,int posy)
{
  int i,j,xspacing,yspacing,tempx,tempy;
  Grid * temp = gf2d_grid_new_grid();
  ButtonInfo *cell;
  temp->rows = rows;
  temp->columns = columns;
  temp->height = height;
  temp->width = width;
  temp->posx = posx;
  temp->posy = posy;
  xspacing = width/columns;
  yspacing = height/rows;
  tempx = posx;
  tempy = posy;
  for(i=0;i<rows;i++)
  {
    for(j=0;j<columns;j++)
    {
      cell = gf2d_grid_init_cell(tempx,tempy,xspacing,yspacing);
      tempx = tempx + xspacing;
      temp->cells = gfc_list_append(temp->cells,cell);
    }
    tempx = posx;
    tempy = tempy+yspacing;
  }
  temp->xspacing = xspacing;
  temp->yspacing = yspacing;
  return temp;
}

ButtonInfo * gf2d_grid_init_cell(int posx,int posy,int width,int height)
{
  ButtonInfo *cell = gf2d_ui_button_new();
  BoxInfo * box = gf2d_ui_box_init_box(NULL,0,0,width,height,1,1,posx,posy,2,vector4d(0,0,0,0),0);
  box->render = 0;
  cell->boxInfo = box;
  gf2d_ui_button_setup_collision_body(cell);
  cell->onRelease = &gf2d_grid_set_sprite;
  cell->onHold = NULL;
  return cell;
}

void gf2d_grid_set_sprite(void * cell)
{
  ButtonInfo * butt = (ButtonInfo*)cell;
  butt->boxInfo->s = gf2d_level_editor_get_selected_sprite();
  butt->boxInfo->render = 1;
  butt->boxInfo->sprite_num =gf2d_level_editor_get_selected_sprite_num();
  butt->boxInfo->resizex = (float)(butt->boxInfo->width)/(float)(butt->boxInfo->s->frame_w);
  butt->boxInfo->resizey = (float)(butt->boxInfo->height)/(float)(butt->boxInfo->s->frame_h);
}

void gf2d_grid_clear_cell(void *cell)
{
  ButtonInfo * butt = (ButtonInfo*)cell;
  butt->boxInfo->render = 0;
  //(ButtonInfo*)cell->s = NULL;
}

void gf2d_grid_draw(Grid *g)
{
  Vector2D p1,p2;
  SDL_Rect rect;
  Vector4D color = {0,0,0,255};
  int num_lines,i;
  num_lines = g->columns-1;
  p1.y = g->posy;
  p2.y = g->posy + g->height;
  for(i=0;i<num_lines;i++)
  {
    p1.x = g->posx+(g->xspacing*(i+1));
    p2.x = p1.x;
    gf2d_draw_line(p1,p2,color);
  }
  num_lines = g->rows-1;
  p1.x = g->posx;
  p2.x = g->posx + g->width;
  for(i=0;i<num_lines;i++)
  {
    p1.y = g->posy+(g->yspacing*(i+1));
    p2.y = p1.y;
    gf2d_draw_line(p1,p2,color);
  }
  rect.x = g->posx;
  rect.y = g->posy;
  rect.w = g->width;
  rect.h = g->height;

  gf2d_draw_rect(rect,color);
}

void gf2d_grid_clear()
{
  int i,count;
  Grid * g;
  count = gfc_list_get_count(grids);
  for(i=0;i<count;i++)
  {
    g = gfc_list_get_nth(grids,i);
    free(g->cells);
    free(g);
  }
  free(grids);
}
