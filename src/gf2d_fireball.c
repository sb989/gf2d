#include "gf2d_fireball.h"


static List * fireballs = NULL;

Projectile * gf2d_fireball_init(int player)
{
  Projectile * fireball = NULL ;
  if(!fireballs)
  {
    fireballs = gfc_list_new();
    atexit(gf2d_fireball_close);
  }
  fireball = gf2d_fireball_new(player);
  fireball->originalPos = gf2d_player_get_pos();
  //fireball->ent->position = fireball->originalPos;
  fireball->ent->update = &gf2d_projectile_update;
  fireball->ent->animate = &gf2d_projectile_animate;
  fireball->offsetAngle = 90;
  fireball->xoffset = 0;
  fireball->yoffset = 0;
  fireball->startFrame = fireball->ent->frame = 0;
  fireball->shadowFrame = -1;
  fireball->count = &gf2d_fireball_lower_count;
  fireball->destStart = 4;
  return fireball;
}

void gf2d_fireball_lower_count(void * p)
{
  Player *player;
  Projectile *proj = (Projectile *)p;
  player = gf2d_player_get_player(proj->owner);
  player->fireball_count = player->fireball_count -1;
}

Projectile * gf2d_fireball_new(int player)
{
  Sprite *f = gf2d_sprite_load_all("images/shot_fireball.png",32,32,4);
  Projectile * fireball = gf2d_projectile_init_projectile(f,FIREBALL,gf2d_fireball_filter(),2,2,4,player,gf2d_player_get_pos(),300);
  //fireballs = gfc_list_append(fireballs,fireball);
  return fireball;
}

Projectile * gf2d_fireball_shoot(int player)
{
  Projectile * fireball = gf2d_fireball_init(player);
  gf2d_projectile_shoot(300,fireball);
  return fireball;
}

void gf2d_fireball_animate()
{
  int i,count;
  count = gfc_list_get_count(fireballs);
  Projectile * fireball;
  for(i=0;i<count;i++)
  {
    fireball = gfc_list_get_nth(fireballs,i);
    if(fireball->_inuse == 1)
      gf2d_projectile_animate(fireball);
  }
}

cpShapeFilter gf2d_fireball_filter()
{
  cpGroup group;
  cpBitmask mask;
  cpBitmask cat;
  cpShapeFilter filter;
  group = 1;
  mask = ENEMIES|OBJECTS;
  cat = FIREBALL;
  filter = cpShapeFilterNew(group,cat,mask);
  return filter;
}

void gf2d_fireball_close()
{
  free(fireballs);
}
