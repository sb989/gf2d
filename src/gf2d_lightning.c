#include "gf2d_lightning.h"


static List * bolts = NULL;

Beam * gf2d_lightning_init()
{
  Beam * lightning;
  //if(!bolts)
  //{
  //  bolts = gfc_list_new();
  //  atexit(gf2d_lightning_close);
  //}
  lightning = gf2d_lightning_new();
  //bolts = gfc_list_append(bolts,lightning);
  lightning->offsetAngle = -270;
  lightning->xoffset = -30;
  lightning->yoffset = -230;
  return lightning;
}

Beam * gf2d_lightning_new()
{
  Sprite *l = gf2d_sprite_load_all("images/skinny_lightning.png",63.875,250,8);
  Beam * lightning = gf2d_beam_init_beam(l,LIGHTNING,gf2d_lightning_filter(),230,1,1,8,5);

  return lightning;
}

cpShapeFilter gf2d_lightning_filter()
{
  cpGroup group;
  cpBitmask mask;
  cpBitmask cat;
  cpShapeFilter filter;
  group = 1;
  mask = ENEMIES;
  cat = LIGHTNING;
  filter = cpShapeFilterNew(group,cat,mask);
  return filter;
}

void gf2d_lightning_close()
{
  free(bolts);
}
