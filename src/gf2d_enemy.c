#include "gf2d_enemy.h"
#include "gf2d_draw.h"
#include "gf2d_player.h"
#include <math.h>
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
  e->animate = &gf2d_enemy_draw;
  e->update = &gf2d_enemy_update;
  e->hp = 20;
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

void gf2d_enemy_draw(void *en)
{
  gf2d_entity_draw(en);
  gf2d_enemy_draw_bb(en);

}

void gf2d_enemy_draw_bb_all()
{
  int i,count;
  Entity *e;
  count = gfc_list_get_count(enemies);
  for(i =0;i<count;i++)
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

void gf2d_enemy_take_damage(int dmg,Entity *e)
{
  e->hp = e->hp - dmg;
  if(e->hp ==0)
  {
    e->_inuse = 0;
    gfc_list_delete_data(enemies,e);
  }
}

void gf2d_enemy_drop_coin(cpSpace *space, Entity *e, void *unused)
{
  if(e->hp <=0)
  {
    Sprite *s = gf2d_sprite_load_all("images/coin.jpg",800,800,1);
    Entity * c = gf2d_entity_new("coin",s,e->position,COIN,gf2d_coin_filter(),vector2d(.01,.01),vector2d(.01*s->frame_w,.01*s->frame_h));
    c->update = &gf2d_entity_update_stationary;
    c->_inuse = 1;
    c->colorShift = e->colorShift;
  }

}

void gf2d_enemy_update(void *enemy)
{
  Entity *e = (Entity *)enemy;
  cpVect vel;
  vel = cpBodyGetVelocity(e->body);
  if(e->knockback == 1 && (fabs(vel.x) > 50 || fabs(vel.y) > 50) )
  {
    //slog("vel too high ");
    cpBodySetVelocity(e->body,cpvmult(vel,0.8));
    return;
  }
  else if (e->knockback == 1 && (fabs(vel.x) <= 50 && fabs(vel.y) <= 50) )
  {
    vel.x =vel.y = 0;
    //slog("vel low enough returning control");
    cpBodySetVelocity(e->body,vel);
    e->knockback = 0;
  }
  if(e->_inuse == 0)
  {
    vel.x = vel.y = 0;
    cpBodySetVelocity(e->body,vel);
    vel.x = vel.y = -1;
    cpBodySetPosition(e->body,vel);
    e->position.x = 0;
    e->position.y = 0;
    return;
  }
  if(gf2d_main_game_get_paused() == 1)
  {
    vel.x = 0;
    vel.y = 0;
    cpBodySetVelocity(e->body,vel);
    return;
  }
  //slog("moving enemy");
  cpVect pos = cpBodyGetPosition(e->body);
  cpVect playerPos = cpBodyGetPosition(gf2d_player_get_player(0)->ent->body);
  if(cpvdist(pos,playerPos)<200)
  {
    vel = cpvsub(pos,playerPos);
    vel = cpvnormalize(vel);
    vel = cpvmult(vel,70);
    vel = cpvneg(vel);
    vel = cpvadd(vel,gf2d_main_game_get_velocity_offset());
    cpBodySetVelocity(e->body,vel);
  }
  else
  {
    vel.x = 0;
    vel.y = 0;
    vel = cpvadd(vel,gf2d_main_game_get_velocity_offset());
    cpBodySetVelocity(e->body,vel);
  }
  e->position.x = pos.x;
  e->position.y = pos.y;

  //slog("vel is %f %f",vel.x,vel.y);
}

cpShapeFilter gf2d_enemy_filter()
{
  cpGroup group;
  cpBitmask mask;
  cpBitmask cat;
  cpShapeFilter filter;
  group = 2;
  mask = PLAYER|LIGHTNING|FIRE|WIND|ROCK|WATER|FIREBALL|ICICLE|OBJECTS;
  cat = ENEMIES;
  filter = cpShapeFilterNew(group,cat,mask);
  return filter;
}

cpShapeFilter gf2d_coin_filter()
{
  cpGroup group;
  cpBitmask mask;
  cpBitmask cat;
  cpShapeFilter filter;
  group = 2;
  mask = PLAYER;
  cat = COIN;
  filter = cpShapeFilterNew(group,cat,mask);
  return filter;
}

void gf2d_enemy_clear()
{
  int i,count;
  Entity *e;
  count = gfc_list_get_count(enemies);
  for(i=0;i<count;i++)
  {
    e = gfc_list_get_nth(enemies,0);
    e->_inuse = 0;
    gfc_list_delete_nth(enemies,0);
  }

}

void gf2d_enemy_close()
{
  free(enemies);
}
