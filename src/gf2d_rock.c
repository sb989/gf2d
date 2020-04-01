#include "gf2d_rock.h"

static List * rocks = NULL;

Projectile * gf2d_rock_init(int player)
{
  Projectile * rock = NULL ;
  if(!rocks)
  {
    rocks = gfc_list_new();
    atexit(gf2d_rock_close);
  }
  rock = gf2d_rock_new(player);
  rock->originalPos = gf2d_player_get_pos();
  //rock->ent->position = rock->originalPos;
  rock->ent->update = &gf2d_projectile_update;
  rock->ent->animate = &gf2d_projectile_animate;
  rock->offsetAngle = 0;
  rock->xoffset = 0;
  rock->yoffset = -30;
  rock->startFrame = rock->ent->frame = 1;
  rock->shadowFrame = 0;
  rock->destStart = 5;
  rock->count = &gf2d_rock_lower_count;
  return rock;
}

void gf2d_rock_lower_count(void * p)
{
  Player *player;
  Projectile * proj = (Projectile*)p;
  player = gf2d_player_get_player(proj->owner);
  player->rock_count = player->rock_count -1;
}

Projectile * gf2d_rock_new(int player)
{
  Sprite *r = gf2d_sprite_load_all("images/rock.png",63,61,7);
  Projectile * rock = gf2d_projectile_init_projectile(r,ROCK,gf2d_rock_filter(),1,1,7,player,gf2d_player_get_pos(),150);
  //rocks = gfc_list_append(rocks,rock);
  return rock;
}

Projectile * gf2d_rock_shoot(int player)
{
  Projectile * rock = gf2d_rock_init(player);
  gf2d_projectile_shoot(100,rock);
  return rock;
}

void gf2d_rock_animate()
{
  int i,count;
  count = gfc_list_get_count(rocks);
  Projectile * rock;
  for(i=0;i<count;i++)
  {
    rock = gfc_list_get_nth(rocks,i);
    if(rock->_inuse == 1)
      gf2d_projectile_animate(rock);
  }
}

cpShapeFilter gf2d_rock_filter()
{
  cpGroup group;
  cpBitmask mask;
  cpBitmask cat;
  cpShapeFilter filter;
  group = 1;
  mask = ENEMIES;
  cat = ROCK;
  filter = cpShapeFilterNew(group,cat,mask);
  return filter;
}

void gf2d_rock_close()
{
  free(rocks);
}
