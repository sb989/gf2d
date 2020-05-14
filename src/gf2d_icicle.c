#include "gf2d_icicle.h"

static List * icicles = NULL;

Projectile * gf2d_icicle_init(int player)
{
  Projectile * icicle = NULL ;
  if(!icicles)
  {
    icicles = gfc_list_new();
    atexit(gf2d_icicle_close);
  }
  icicle = gf2d_icicle_new(player);
  icicle->originalPos = gf2d_player_get_pos();
  //icicle->ent->position = icicle->originalPos;
  icicle->ent->update = &gf2d_projectile_update;
  icicle->ent->animate = &gf2d_projectile_animate;
  icicle->offsetAngle = 0;
  icicle->xoffset = 0;
  icicle->yoffset = 0;
  icicle->startFrame = icicle->ent->frame = 32;
  icicle->shadowFrame = -1;
  icicle->count = &gf2d_icicle_lower_count;
  icicle->destStart = 39;
  return icicle;
}

void gf2d_icicle_lower_count(void * p)
{
  Player *player;
  Projectile *proj = (Projectile *)p;
  player = gf2d_player_get_player(proj->owner);
  player->icicle_count = player->icicle_count -1;
}

Projectile * gf2d_icicle_new(int player)
{
  Sprite *i = gf2d_sprite_load_all("images/icicle_0.png",64,64,8);
  Projectile * icicle = gf2d_projectile_init_projectile(i,ICICLE,gf2d_icicle_filter(),1,1,40,player,gf2d_player_get_pos(),500);
  //icicles = gfc_list_append(icicles,icicle);
  return icicle;
}

Projectile * gf2d_icicle_shoot(int player)
{
  Projectile * icicle = gf2d_icicle_init(player);
  gf2d_projectile_shoot(300,icicle);
  return icicle;
}

void gf2d_icicle_animate()
{
  int i,count;
  count = gfc_list_get_count(icicles);
  Projectile * icicle;
  for(i=0;i<count;i++)
  {
    icicle = gfc_list_get_nth(icicles,i);
    if(icicle->_inuse == 1)
      gf2d_projectile_animate(icicle);
  }
}

cpShapeFilter gf2d_icicle_filter()
{
  cpGroup group;
  cpBitmask mask;
  cpBitmask cat;
  cpShapeFilter filter;
  group = 1;
  mask = ENEMIES|OBJECTS;
  cat = ICICLE;
  filter = cpShapeFilterNew(group,cat,mask);
  return filter;
}

void gf2d_icicle_close()
{
  free(icicles);
}
