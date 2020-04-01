#include "gf2d_fire.h"

static List * fires = NULL;

Beam * gf2d_fire_init()
{
  Beam * fire;
  //if(!fires)
//  {
//    fires = gfc_list_new();
  //  atexit(gf2d_fire_close);
//  }
  fire = gf2d_fire_new();
//  fires = gfc_list_append(fires,fire);
  fire->offsetAngle = -270;
  fire->xoffset = -30;
  fire->yoffset = -180;
  return fire;
}

Beam * gf2d_fire_new()
{
  Sprite *f = gf2d_sprite_load_all("images/shoot_fire_spritesheet2.png",26,60,61);
  Beam * fire = gf2d_beam_init_beam(f,FIRE,gf2d_fire_filter(),180,3,3,61,10);
  return fire;
}

void gf2d_fire_animate( )
{
  int i,count;
  count = gfc_list_get_count(fires);
  Beam * fire;
  for(i=0;i<count;i++)
  {
    fire = gfc_list_get_nth(fires,i);
    if(fire->_inuse == 1)
      gf2d_beam_animate(fire);
  }
}

cpShapeFilter gf2d_fire_filter()
{
  cpGroup group;
  cpBitmask mask;
  cpBitmask cat;
  cpShapeFilter filter;
  group = 1;
  mask = ENEMIES;
  cat = FIRE;
  filter = cpShapeFilterNew(group,cat,mask);
  return filter;
}

void gf2d_fire_close()
{
  free(fires);
}
