#include "gf2d_enemy.h"

static List * enemies = NULL;

void gf2d_enemy_init()
{
  enemies = gfc_list_new();
  atexit(gf2d_enemy_close);
}

void gf2d_enemy_init_enemy(Vector2D pos,char * color)
{
  Entity *e;
  Vector4D * c = (Vector4D*)malloc(sizeof(Vector4D));
  Sprite *s = gf2d_sprite_load_all("images/tilemap_packed.png",16,16,27);
  e = gf2d_entity_new("enemy",s,pos,ENEMIES,gf2d_enemy_filter(),vector2d(2,2),vector2d(2*s->frame_w,2*s->frame_h));
  e->frame = 307;
  e->animate = &gf2d_entity_draw;
  //e->scale = (Vector2D*)malloc(sizeof(Vector2D));
  //e->scale->x = 2;
  //e->scale->y = 2;
  e->_inuse = 1;
  if(strcmp(color,"red")==0)
  {
    c->x = c->w = 255;
    c->y = c->z = 0;
  }
  else if(strcmp(color,"green")==0)
  {
    c->y = c->w = 255;
    c->x = c->z = 0;
  }
  else if(strcmp(color,"blue")==0)
  {
    c->z = c->w = 255;
    c->x = c->y = 0;
  }
  else if(strcmp(color,"yellow")==0)
  {
    c->x = c->y = c->w =255;
    c->z = 0;
  }
  else if(strcmp(color,"orange")==0)
  {
    c->x = c->w = 255;
    c->y = 165;
    c->z = 0;

  }
  else if(strcmp(color,"purple")==0)
  {
    c->x = c->z = 128;
    c->w = 255;
    c->y =0;
  }
  else if(strcmp(color,"grey")==0)
  {
    c->x =c->y = c->z=128;
    c->w = 255;
  }
  else
  {
    c->z=c->y=c->x = c->w = 255;
  }
  e->colorShift = c;
  enemies = gfc_list_append(enemies,e);
}

void gf2d_enemy_draw_bb_all()
{
  int i,count;
  Entity *e;
  count = gfc_list_get_count(enemies);
  for(int i =0;i<count;i++)
  {
    e = gfc_list_get_nth(enemies,i);
    gf2d_enemy_draw_bb(e);
  }
}

void gf2d_enemy_draw_bb(Entity *e)
{
  cpBB bb= cpShapeGetBB(e->shape);
  SDL_Rect rect;
  Vector4D color;
  color.x = color.y = color.z = 0;
  color.w = 255;
  rect.x = bb.l;
  rect.y = bb.b;
  rect.w = bb.r - bb.l;
  rect.h = abs(bb.t - bb.b);
  gf2d_draw_rect(rect,color);
}

cpShapeFilter gf2d_enemy_filter()
{
  cpGroup group;
  cpBitmask mask;
  cpBitmask cat;
  cpShapeFilter filter;
  group = 0;
  mask = PLAYER;
  cat = ENEMIES;
  filter = cpShapeFilterNew(group,cat,mask);
  return filter;
}

void gf2d_enemy_close()
{
  free(enemies);
}
