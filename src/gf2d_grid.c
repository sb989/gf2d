#include "gf2d_grid.h"

static List * grids = NULL;
static int grid_count;
static int current_grid;
static int right_clicked;
static int left_clicked;
void gf2d_grid_init()
{
  grids = gfc_list_new();
  grid_count =0;
  current_grid = 0;
  right_clicked = 0;
  left_clicked = 0;
  atexit(gf2d_grid_close);
}

Grid * gf2d_grid_new_grid()
{
  Grid * temp = (Grid*)malloc(sizeof(Grid));
  temp->cells = gfc_list_new();
  temp->gridnum = grid_count;
  temp->isEmpty = 1;

  grid_count = grid_count +1;
  grids = gfc_list_append(grids,temp);
  return temp;
}

Grid * gf2d_grid_init_grid(int rows,int columns,int height,int width,int posx,int posy)
{
  int i,j,xspacing,yspacing,tempx,tempy;
  Grid * temp = gf2d_grid_new_grid();
  ButtonInfo *cell;
  Sprite *s;
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
      cell->holdernum = temp->gridnum;
      tempx = tempx + xspacing;
      temp->cells = gfc_list_append(temp->cells,cell);
    }
    tempx = posx;
    tempy = tempy+yspacing;
  }
  temp->xspacing = xspacing;
  temp->yspacing = yspacing;

  s = gf2d_sprite_load_all("images/dotBlue.png",24,24,1);
  temp->left = gf2d_image_carousel_button_init(s,0,posx-34,posy+(height/2),NULL,&gf2d_grid_left_clicked,1,1);//second NULL is onRelease
  temp->right = gf2d_image_carousel_button_init(s,0,posx+width+10,posy+(height/2),NULL,&gf2d_grid_right_clicked,1,1);//second NULL is onRelease
  temp->left->boxInfo->render = 0;
  temp->right->boxInfo->render = 0;
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
  cell->onHold = &gf2d_grid_set_sprite;
  return cell;
}

void gf2d_grid_set_render_zero(Grid *g)
{
  int i,count;
  ButtonInfo * cell;
  count = gfc_list_get_count(g->cells);
  for(i=0;i<count;i++)
  {
    cell = gfc_list_get_nth(g->cells,i);
    cell->boxInfo->render = 0;
  }
}

void gf2d_grid_set_render_one(Grid *g)
{
  int i,count;
  ButtonInfo * cell;
  count = gfc_list_get_count(g->cells);
  for(i=0;i<count;i++)
  {
    cell = gfc_list_get_nth(g->cells,i);
    if(cell->boxInfo->s)
      cell->boxInfo->render =1;
  }
}

void gf2d_grid_set_sprite(void * cell)
{
  Grid *g;
  ButtonInfo * butt = (ButtonInfo*)cell;
  if(butt->holdernum != current_grid)
    return;
  butt->boxInfo->s = gf2d_level_editor_get_selected_sprite();
  if(!butt->boxInfo->s)
    return;
  butt->boxInfo->render = 1;
  butt->boxInfo->sprite_num =gf2d_level_editor_get_selected_sprite_num();
  butt->boxInfo->resizex = (float)(butt->boxInfo->width)/(float)(butt->boxInfo->s->frame_w);
  butt->boxInfo->resizey = (float)(butt->boxInfo->height)/(float)(butt->boxInfo->s->frame_h);
  g = (Grid*)gfc_list_get_nth(grids,butt->holdernum);
  g->isEmpty = 0;
}

void gf2d_grid_clear_cell(void *cell)
{
  ButtonInfo * butt = (ButtonInfo*)cell;
  butt->boxInfo->render = 0;
  //(ButtonInfo*)cell->s = NULL;
}

void gf2d_grid_draw()
{
  Grid *g = gfc_list_get_nth(grids,current_grid);
  if(g)
  {
    gf2d_grid_draw_grid(g);
    gf2d_grid_draw_buttons(g);
  }
  else
  {
    slog("no grid");
  }

}

void gf2d_grid_draw_buttons(Grid *g)
{
  Vector2D * v2;
  Vector4D *l,*r;
  if(g->isEmpty == 1 && g->gridnum == 0)
  {
    l = (Vector4D*)malloc(sizeof(Vector4D));
    l->x = 255;
    l->y = 255;
    l->z = 255;
    l->w = 100;
    r =l;
  }
  else if(g->isEmpty == 0 && g->gridnum == 0)
  {
    l = (Vector4D*)malloc(sizeof(Vector4D));
    l->x = 255;
    l->y = 255;
    l->z = 255;
    l->w = 100;
    r = NULL;
  }
  else if(g->isEmpty == 0 && g->gridnum >0)
  {
    l = NULL;
    r = NULL;
  }
  else if(g->isEmpty ==1 && g->gridnum>0)
  {
    l = NULL;
    r = (Vector4D*)malloc(sizeof(Vector4D));
    r->x = 255;
    r->y = 255;
    r->z = 255;
    r->w = 100;
  }
  BoxInfo * s = g->left->boxInfo;
  v2 = (Vector2D * )malloc(sizeof(Vector2D));
  v2->x = (float)s->resizex;
  v2->y = (float)s->resizey;
  gf2d_sprite_draw(s->s,vector2d(s->posx,s->posy),v2,NULL,
  NULL,NULL,NULL,l,s->sprite_num);

  s = g->right->boxInfo;
  v2->x = (float)s->resizex;
  v2->y = (float)s->resizey;
  gf2d_sprite_draw(s->s,vector2d(s->posx,s->posy),v2,NULL,
  NULL,NULL,NULL,r,s->sprite_num);
  if(l)free(l);
  if(r && r!=l)free(r);
  free(v2);
}

void gf2d_grid_draw_grid(Grid *g)
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

void gf2d_grid_left_button()
{

  Grid *g = gfc_list_get_nth(grids,current_grid);
  left_clicked = 0;
  if(g->isEmpty == 1 && g->gridnum == 0)
  {
    return;
  }
  else if(g->isEmpty == 0 && g->gridnum == 0)
  {
    return;
  }
  else if(g->isEmpty == 0 && g->gridnum >0)
  {
    gf2d_grid_set_render_zero(g);
    current_grid = current_grid-1;
    g = gfc_list_get_nth(grids,current_grid);
    gf2d_grid_set_render_one(g);
  }
  else if(g->isEmpty ==1 && g->gridnum>0)
  {
    gf2d_grid_set_render_zero(g);
    current_grid = current_grid-1;
    g = gfc_list_get_nth(grids,current_grid);
    gf2d_grid_set_render_one(g);
  }
}

int gf2d_grid_get_right_clicked()
{
  return right_clicked;
}

void gf2d_grid_right_clicked(void * nothing)
{
  gf2d_set_left_released(false);
  right_clicked = SDL_GetTicks();
}

int gf2d_grid_get_left_clicked()
{
  //slog("left clicked is %d",left_clicked);
  return left_clicked;
}

void gf2d_grid_left_clicked(void * nothing)
{
  gf2d_set_left_released(false);
  left_clicked =SDL_GetTicks();
}

void gf2d_grid_right_button()
{
  int count = gfc_list_get_count(grids);
  Grid *g = gfc_list_get_nth(grids,current_grid);
  right_clicked =0;
  if(g->isEmpty == 1 && g->gridnum == 0)
  {
    return;
  }
  else if(g->isEmpty == 0 && g->gridnum == 0)
  {
    gf2d_grid_set_render_zero(g);
    if(count-1 == g->gridnum)
    {
      gf2d_grid_init_grid(g->rows,g->columns,g->height,g->width,g->posx,g->posy);
      current_grid = current_grid +1;
    }
    else
    {
      current_grid = current_grid+1;
      g = gfc_list_get_nth(grids,current_grid);
      gf2d_grid_set_render_one(g);
    }
  }
  else if(g->isEmpty == 0 && g->gridnum >0)
  {
    gf2d_grid_set_render_zero(g);
    if(count-1 == g->gridnum)
    {
      gf2d_grid_init_grid(g->rows,g->columns,g->height,g->width,g->posx,g->posy);
      current_grid = current_grid +1;
    }
    else
    {
      current_grid = current_grid+1;
      g = gfc_list_get_nth(grids,current_grid);
      gf2d_grid_set_render_one(g);
    }
  }
  else if(g->isEmpty ==1 && g->gridnum>0)
  {
    return;
  }
}

List * gf2d_grid_return_all()
{
  return grids;
}


void gf2d_grid_clear_list()
{
  int i,count;
  Grid * g;
  count = gfc_list_get_count(grids);
  for(i=0;i<count;i++)
  {
    g = gfc_list_get_nth(grids,i);
    slog("i=%d %p",i,g);
    free(g->cells);
    free(g);
  }
  free(grids);
  grids = gfc_list_new();
  grid_count =0;
  current_grid = 0;
  right_clicked = 0;
  left_clicked = 0;
}

void gf2d_grid_close()
{
  int i,count;
  Grid * g;
  count = gfc_list_get_count(grids);
  for(i=0;i<count;i++)
  {
    g = gfc_list_get_nth(grids,i);
    slog("i=%d %p",i,g);
    free(g->cells);
    free(g);
  }
  free(grids);
}
