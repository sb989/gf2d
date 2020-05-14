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
  temp->objects = gfc_list_new();
  temp->hitBoxes = gfc_list_new();
  temp->gridnum = grid_count;
  temp->isEmpty = 0;

  grid_count = grid_count +1;
  grids = gfc_list_append(grids,temp);
  return temp;
}

Grid * gf2d_grid_init_grid(int rows,int columns,int height,int width,int posx,int posy)
{
  int i,j,xspacing,yspacing,tempx,tempy;
  Grid * temp = gf2d_grid_new_grid();
  ButtonInfo *cell;
  ForegroundObject * object;
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
      object = gf2d_grid_init_object(tempx,tempy,xspacing,yspacing);
      cell = gf2d_grid_init_cell(tempx,tempy,xspacing,yspacing);
      cell->holdernum = temp->gridnum;
      tempx = tempx + xspacing;
      temp->cells = gfc_list_append(temp->cells,cell);
      temp->objects = gfc_list_append(temp->objects,object);
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

ForegroundObject * gf2d_grid_new_foreground_object()
{
  ForegroundObject * temp = (ForegroundObject*)malloc(sizeof(ForegroundObject));
  return temp;
}

ObjectHitBox * gf2d_grid_new_object_hitbox()
{
  ObjectHitBox * temp = (ObjectHitBox*)malloc(sizeof(ObjectHitBox));
  return temp;
}

ForegroundObject * gf2d_grid_init_object(int posx, int posy, int width, int height)
{
  BoxInfo * box = gf2d_ui_box_init_box(NULL,0,0,width,height,1,1,posx,posy,2,vector4d(0,0,0,0),0);
  box->render = 0;
  ForegroundObject * obj = gf2d_grid_new_foreground_object();
  obj->box = box;
  obj->render= 0;
  return obj;
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
  cell->rightOnRelease = &gf2d_grid_clear_sprite;
  cell->rightOnHold = &gf2d_grid_clear_sprite;
  return cell;
}

ObjectHitBox * gf2d_grid_init_object_hitbox(int posx,int posy,int width,int height,char * name)
{
  BoxInfo * box = gf2d_ui_box_init_box(NULL,0,0,width,height,1,1,posx,posy,0,vector4d(255,0,0,50),0);
  box->render = 0;
  ObjectHitBox * obj = gf2d_grid_new_object_hitbox();
  obj->box = box;
  obj->render= 0;
  obj->name = name;
  return obj;
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
  ButtonInfo * butt = (ButtonInfo*)cell;
  if(butt->holdernum != current_grid)
    return;
  if(gf2d_level_editor_get_edit_mode() == background)
  {
    gf2d_grid_set_background_sprite(cell);
  }
  else
  {
    gf2d_grid_set_forground_sprite(cell);
  }
}

void gf2d_grid_set_background_sprite(void * cell)
{
  Grid *g;
  ButtonInfo * butt = (ButtonInfo*)cell;
  Sprite * s = gf2d_level_editor_get_selected_sprite();

  if(!s)
    return;
  g = (Grid*)gfc_list_get_nth(grids,butt->holdernum);
  if(butt->boxInfo->render == 0)
    g->isEmpty++;
  butt->boxInfo->s = s;
  butt->boxInfo->render = 1;
  butt->boxInfo->sprite_num =gf2d_level_editor_get_selected_sprite_num();
  butt->boxInfo->resizex = (float)(butt->boxInfo->width)/(float)(butt->boxInfo->s->frame_w);
  butt->boxInfo->resizey = (float)(butt->boxInfo->height)/(float)(butt->boxInfo->s->frame_h);

}

void gf2d_grid_set_forground_sprite(void * cell)
{
  Grid *g;
  ButtonInfo * butt = (ButtonInfo*)cell;
  ForegroundObject * obj;
  g = (Grid*)gfc_list_get_nth(grids,butt->holdernum);
  int index = ((butt->boxInfo->posx - g->posx)/g->xspacing) + ((butt->boxInfo->posy - g->posy)/g->yspacing)*g->columns;
  obj = (ForegroundObject*)gfc_list_get_nth(g->objects,index);
  Sprite * s;
  s = gf2d_level_editor_get_selected_sprite();
  if(!s)
    return;
  if(obj->render == 0)
    g->isEmpty++;
  obj->box->s = s;
  obj->render = 1;
  obj->box->sprite_num = gf2d_level_editor_get_selected_sprite_num();
  obj->box->resizex = (float)(obj->box->width)/(float)(obj->box->s->frame_w);
  obj->box->resizey = (float)(obj->box->height)/(float)(obj->box->s->frame_h);

}

void gf2d_grid_clear_sprite(void * cell)
{
  ButtonInfo * butt = (ButtonInfo*)cell;
  if(butt->holdernum != current_grid)
    return;
  if(gf2d_level_editor_get_edit_mode() == background)
  {
    gf2d_grid_clear_background_sprite(cell);
  }
  else
  {
    gf2d_grid_clear_forground_sprite(cell);
  }
}

void gf2d_grid_clear_background_sprite(void * cell)
{
  Grid *g;
  ButtonInfo * butt = (ButtonInfo*)cell;
  if(butt->boxInfo->s == NULL)
    return;
  butt->boxInfo->s = NULL;
  g = (Grid*)gfc_list_get_nth(grids,butt->holdernum);
  if(g->isEmpty>0)
    g->isEmpty --;
}

void gf2d_grid_clear_forground_sprite(void * cell)
{
  Grid *g;
  ButtonInfo * butt = (ButtonInfo*)cell;
  ForegroundObject * obj;
  g = (Grid*)gfc_list_get_nth(grids,butt->holdernum);
  int index = ((butt->boxInfo->posx - g->posx)/g->xspacing) + ((butt->boxInfo->posy - g->posy)/g->yspacing)*g->columns;
  obj = (ForegroundObject*)gfc_list_get_nth(g->objects,index);
  if(obj->box->s == NULL)
    return;

  obj->box->s = NULL;
  if(g->isEmpty>0)
    g->isEmpty--;
}

void gf2d_grid_create_object_hitbox()
{
  int posx,posy,width,height;
  char * name;
  //ButtonInfo * butt = (ButtonInfo*)cell;
  Grid * g = (Grid*)gfc_list_get_nth(grids,current_grid);
  posx = gf2d_level_editor_get_new_hitbox_posx();
  posy = gf2d_level_editor_get_new_hitbox_posy();
  width = gf2d_level_editor_get_new_hitbox_width();
  height = gf2d_level_editor_get_new_hitbox_height();
  name = gf2d_level_editor_get_new_hitbox_name();

  posx = g->posx + (posx * g->xspacing);
  posy = g->posy + (posy * g->yspacing);
  height = height * g->yspacing;
  width = width * g->xspacing;

  ObjectHitBox * hitbox = gf2d_grid_init_object_hitbox(posx,posy,width,height,name);
  g->hitBoxes = gfc_list_append(g->hitBoxes,hitbox);
  hitbox->render = 1;
  slog("created hitbox");
}

void gf2d_grid_destroy_object_hitbox(void * butt)
{


}

void gf2d_grid_draw()
{
  Grid *g = gfc_list_get_nth(grids,current_grid);
  if(g)
  {
    gf2d_grid_draw_objects(g);
    gf2d_grid_draw_object_hitbox(g);
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
  if(g->isEmpty == 0 && g->gridnum == 0)
  {
    l = (Vector4D*)malloc(sizeof(Vector4D));
    l->x = 255;
    l->y = 255;
    l->z = 255;
    l->w = 100;
    r =l;
  }
  else if(g->isEmpty > 0 && g->gridnum == 0)
  {
    l = (Vector4D*)malloc(sizeof(Vector4D));
    l->x = 255;
    l->y = 255;
    l->z = 255;
    l->w = 100;
    r = NULL;
  }
  else if(g->isEmpty > 0 && g->gridnum >0)
  {
    l = NULL;
    r = NULL;
  }
  else if(g->isEmpty == 0 && g->gridnum>0)
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


void gf2d_grid_draw_object_hitbox(Grid *g)
{
  int i,count;
  ObjectHitBox *hitbox;
  count = gfc_list_get_count(g->hitBoxes);
  for(i=0;i<count;i++)
  {
    hitbox = (ObjectHitBox*)gfc_list_get_nth(g->hitBoxes,i);
    gf2d_ui_box_draw(hitbox->box);
  }
}

void gf2d_grid_draw_objects(Grid *g)
{
  int i;
  int count = gfc_list_get_count(g->objects);
  ForegroundObject *obj;
  BoxInfo *box;
  for(i = 0;i<count;i++)
  {
    obj = (ForegroundObject*)gfc_list_get_nth(g->objects,i);
    if(obj->render)
    {
      box = obj->box;
      gf2d_ui_box_draw(box);
    }
  }
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
  if(g->isEmpty == 0 && g->gridnum == 0)
  {
    return;
  }
  else if(g->isEmpty > 0 && g->gridnum == 0)
  {
    return;
  }
  else if(g->isEmpty > 0 && g->gridnum >0)
  {
    gf2d_grid_set_render_zero(g);
    current_grid = current_grid-1;
    g = gfc_list_get_nth(grids,current_grid);
    gf2d_grid_set_render_one(g);
  }
  else if(g->isEmpty ==0 && g->gridnum>0)
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
  if(g->isEmpty == 0 && g->gridnum == 0)
  {
    return;
  }
  else if(g->isEmpty > 0 && g->gridnum == 0)
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
  else if(g->isEmpty > 0 && g->gridnum >0)
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
  else if(g->isEmpty ==0 && g->gridnum>0)
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
    //slog("i=%d %p",i,g);
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
