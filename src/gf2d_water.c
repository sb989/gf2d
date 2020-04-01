#include "gf2d_water.h"

static List * waters = NULL;

Beam * gf2d_water_init()
{
  Beam * water;
  //if(!waters)
//  {
//    waters = gfc_list_new();
  //  atexit(gf2d_water_close);
//  }
  water = gf2d_water_new();
//  waters = gfc_list_append(waters,water);
  Vector4D * color = (Vector4D*)malloc(sizeof(Vector4D));
  color->z = color->w = 255;
  color->y = color->x = 0;
  water->offsetAngle = -270;
  water->xoffset = -30;
  water->yoffset = -180;
  water->colorShift = color;
  return water;
}

Beam * gf2d_water_new()
{
  Sprite *w = gf2d_sprite_load_all("images/shoot_fire_spritesheet2.png",26,60,61);
  Beam * water = gf2d_beam_init_beam(w,WATER,gf2d_water_filter(),180,3,3,61,10);
  return water;
}

void gf2d_water_animate( )
{
  int i,count;
  count = gfc_list_get_count(waters);
  Beam * water;
  for(i=0;i<count;i++)
  {
    water = gfc_list_get_nth(waters,i);
    if(water->_inuse == 1)
      gf2d_beam_animate(water);
  }
}

cpShapeFilter gf2d_water_filter()
{
  cpGroup group;
  cpBitmask mask;
  cpBitmask cat;
  cpShapeFilter filter;
  group = 1;
  mask = ENEMIES;
  cat = WATER;
  filter = cpShapeFilterNew(group,cat,mask);
  return filter;
}

void gf2d_water_close()
{
  free(waters);
}
