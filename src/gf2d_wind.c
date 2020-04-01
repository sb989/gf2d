#include "gf2d_wind.h"

static List * winds = NULL;

Beam * gf2d_wind_init()
{
  Beam * wind;

  wind = gf2d_wind_new();


  wind->offsetAngle = -270;
  wind->xoffset = -70;
  wind->yoffset = -310;
  return wind;
}

Beam * gf2d_wind_new()
{
  Sprite *w = gf2d_sprite_load_all("images/wind.png",125,146,9);
  Beam * wind = gf2d_beam_init_beam(w,WIND,gf2d_wind_filter(),240,1,2,9,20);
  return wind;
}

cpShapeFilter gf2d_wind_filter()
{
  cpGroup group;
  cpBitmask mask;
  cpBitmask cat;
  cpShapeFilter filter;
  group = 1;
  mask = ENEMIES;
  cat = WIND;
  filter = cpShapeFilterNew(group,cat,mask);
  return filter;
}

void gf2d_wind_close()
{
  free(winds);
}
