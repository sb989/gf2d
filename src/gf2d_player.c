#include "gf2d_player.h"
#include "gf2d_fire.h"
#include "gf2d_lightning.h"
#include "gf2d_rock.h"
#include "gf2d_wind.h"
#include "gf2d_icicle.h"
#include "gf2d_water.h"
#include "gf2d_fireball.h"
#include <math.h>
static Player *player;
enum directions {left,right,back,forw};
static enum directions direction;
static int playerCount = 0;
enum attack{rock,water,fire,lightning,wind,icicle,fireball,nothing};
static enum attack atk;
//void gf2d_player_movement(TileMap * map);

void gf2d_player_init()
{
  player = gf2d_player_new_player();
  Sprite * s = gf2d_sprite_load_all("images/tilemap_packed.png",16,16,27);
  player->resizex = 1.5*gf2d_tilemap_get_tile_width()/s->frame_w;
  player->resizey = 1.5*gf2d_tilemap_get_tile_height()/s->frame_h;
  player->ent = gf2d_entity_new("player",s,vector2d(50,50),PLAYER,gf2d_player_filter(),vector2d(player->resizex,player->resizey),vector2d(1.5*gf2d_tilemap_get_tile_width(),1.5*gf2d_tilemap_get_tile_height()));
  player->ent->update = NULL;
  player->ent->animate = &gf2d_player_animate;
  player->ent->animateData = player;
  player->ent->_inuse = 1;
  player->forw = 24;
  player->back = 25;
  player->left = (23);
  player->right = (26);
  player->frames_per_animation = 3;
  player->ent->frame = 0;
  player->invincible = 0;
  player->draw_width = 1.5*gf2d_tilemap_get_tile_width();
  player->draw_height = 1.5*gf2d_tilemap_get_tile_height();
  player->wind = gf2d_wind_init();
  player->fire = gf2d_fire_init();
  player->lightning = gf2d_lightning_init();
  player->water = gf2d_water_init();
  player->rock_limit = player->icicle_limit = player->fireball_limit = 1;
  player->rock_count = player->icicle_count = player->fireball_count = 0;
  direction = forw;
  atk = nothing;
  player->playerNum = playerCount;
  playerCount = playerCount +1;
  atexit(gf2d_player_close);
}

void gf2d_player_set_inuse(int inuse)
{
  player->ent->_inuse = inuse;
}

Player * gf2d_player_new_player()
{
  Player * temp = (Player*)malloc(sizeof(Player));
  return temp;
}

void gf2d_player_update_position()
{
  cpVect pos = cpBodyGetPosition(player->ent->body);
  player->ent->position.x = pos.x;
  player->ent->position.y = pos.y;
}

Vector2D gf2d_player_get_pos()
{
  return player->ent->position;
}

void gf2d_player_set_pos(Vector2D pos)
{
  cpVect p;
  p.x = pos.x;
  p.y = pos.y;
  cpBodySetPosition(player->ent->body,p);
  gf2d_player_update_position();
}

float gf2d_player_get_draw_height()
{
  return player->draw_height;
}

float gf2d_player_get_draw_width()
{
  return player->draw_width;
}

void gf2d_player_set_x_velocity(Vector2D velocity)
{
  cpVect pos;
  pos.x = velocity.x;
  //pos.y = velocity.y;
  cpBodySetVelocity(player->ent->body,pos);
}

void gf2d_player_set_y_velocity(Vector2D velocity)
{
  cpVect pos;
  //pos.x = velocity.x;
  pos.y = velocity.y;
  cpBodySetVelocity(player->ent->body,pos);
}

void gf2d_player_set_forw()
{
  direction = forw;
}
void gf2d_player_set_back()
{
  direction = back;
}
void gf2d_player_set_left()
{
  direction = left;
}
void gf2d_player_set_right()
{
  direction = right;
}

void gf2d_player_animate(void * nothing)
{
  int f;
  Vector2D * v2;

  if(gf2d_game_state_get_state() == 0 || gf2d_game_state_get_state()== 4)
  {
    switch(direction)
    {
      case forw:
        f = 27*(int)player->ent->frame+player->forw;
        break;
      case back:
        f = 27*(int)player->ent->frame+player->back;
        break;
      case left:
        f = 27*(int)player->ent->frame+player->left;
        break;
      case right:
        f=27*(int)player->ent->frame+player->right;
        break;
      default:
        break;
    }
    v2 = (Vector2D * )malloc(sizeof(Vector2D));
    v2->x = player->resizex;
    v2->y = player->resizey;
    if(player->invincible > 0 && (player->invincible/10)%2 ==1)
    {
      Vector4D *color  = (Vector4D *)malloc(sizeof(Vector4D));
      color->x = color->y = color->z = 255;
      color->w = 0;
      gf2d_sprite_draw(player->ent->s,player->ent->position,v2,NULL,NULL,NULL,NULL,color,f);
      free(color);
    }
    else
    {
      gf2d_sprite_draw(player->ent->s,player->ent->position,v2,NULL,NULL,NULL,NULL,NULL,f);
    }

    if(player->invincible > 0 && player->invincible < 100 )
      player->invincible = player->invincible +1;
    else if(player->invincible >= 100)
      player->invincible = 0;
    free(v2);
    gf2d_player_draw_bb();
    if(player->ent->frame > player->frames_per_animation-1)
      player->ent->frame = 0;
    else if(player->ent->frame <= player->frames_per_animation-1)
      player->ent->frame = player->ent->frame+.1;
  }
}

void gf2d_player_draw_bb()
{
  cpBB bb= cpShapeGetBB(player->ent->shape);
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

void gf2d_player_set_attack()
{

  if(!gf2d_key_pressed(SDL_SCANCODE_Q) && !gf2d_key_pressed(SDL_SCANCODE_W) && !gf2d_key_pressed(SDL_SCANCODE_E) && gf2d_key_pressed(SDL_SCANCODE_R) && !gf2d_key_pressed(SDL_SCANCODE_T))
  {
    atk = fire;
  }
  else if(!gf2d_key_pressed(SDL_SCANCODE_Q) && !gf2d_key_pressed(SDL_SCANCODE_W) && !gf2d_key_pressed(SDL_SCANCODE_E) && !gf2d_key_pressed(SDL_SCANCODE_R) && gf2d_key_pressed(SDL_SCANCODE_T))
  {
    atk = lightning;
  }
  else if(!gf2d_key_pressed(SDL_SCANCODE_Q) && !gf2d_key_pressed(SDL_SCANCODE_W) && gf2d_key_pressed(SDL_SCANCODE_E) && !gf2d_key_pressed(SDL_SCANCODE_R) && !gf2d_key_pressed(SDL_SCANCODE_T))
  {
    atk = rock;
  }
  else if(!gf2d_key_pressed(SDL_SCANCODE_Q) && gf2d_key_pressed(SDL_SCANCODE_W) && !gf2d_key_pressed(SDL_SCANCODE_E) && !gf2d_key_pressed(SDL_SCANCODE_R) && !gf2d_key_pressed(SDL_SCANCODE_T))
  {
    atk = wind;
  }
  else if (gf2d_key_pressed(SDL_SCANCODE_Q) && gf2d_key_pressed(SDL_SCANCODE_W) && !gf2d_key_pressed(SDL_SCANCODE_E) && !gf2d_key_pressed(SDL_SCANCODE_R) && !gf2d_key_pressed(SDL_SCANCODE_T))
  {
    atk = icicle;
  }
  else if (!gf2d_key_pressed(SDL_SCANCODE_Q) && !gf2d_key_pressed(SDL_SCANCODE_W) && gf2d_key_pressed(SDL_SCANCODE_E) && gf2d_key_pressed(SDL_SCANCODE_R) && !gf2d_key_pressed(SDL_SCANCODE_T))
  {
    atk = fireball;
  }
  else if(gf2d_key_pressed(SDL_SCANCODE_Q) && !gf2d_key_pressed(SDL_SCANCODE_W) && !gf2d_key_pressed(SDL_SCANCODE_E) && !gf2d_key_pressed(SDL_SCANCODE_R) && !gf2d_key_pressed(SDL_SCANCODE_T))
  {
    atk = water;
  }
  else
  {
    atk = nothing;
  }
}

void gf2d_player_attack()
{
  //slog("atk is %d",atk);
  if(gf2d_right_released())
  {
    //slog("released");
    switch(atk)
    {
      case fire:
        break;
      case water:
        break;
      case wind:
        break;
      case lightning:
        break;
      case rock:
        if(player->rock_count<player->rock_limit)
        {
          player->rock_count +=1;
          gf2d_rock_shoot(player->playerNum);
        }
        break;
      case fireball:
        if(player->fireball_count<player->fireball_limit)
        {
          player->fireball_count+= 1;
          gf2d_fireball_shoot(player->playerNum);
        }
        break;
      case icicle:
        if(player->icicle_count<player->icicle_limit)
        {
          player->icicle_count+= 1;
          gf2d_icicle_shoot(player->playerNum);
        }
        break;
      case nothing:
        player->lightning->shoot =0;
        player->fire->shoot =0;
        player->wind->shoot = 0;
        player->water->shoot = 0;
        break;
    }
  gf2d_set_right_released(false);
  }
  else if(gf2d_right_mouse_down())
  {
    //slog("rdown");
    switch(atk)
    {
      case fire:
        player->fire->shoot = 1;
        player->lightning->shoot =0;
        player->wind->shoot = 0;
        player->water->shoot = 0;
        break;
      case water:
        player->water->shoot = 1;
        player->lightning->shoot =0;
        player->wind->shoot = 0;
        player->fire->shoot =0;
        break;
      case wind:
        player->wind->shoot = 1;
        player->lightning->shoot = 0;
        player->fire->shoot =0;
        player->water->shoot = 0;
        break;
      case lightning:
        player->lightning->shoot = 1;
        player->fire->shoot =0;
        player->wind->shoot = 0;
        player->water->shoot = 0;
        break;
      case icicle:
        break;
      case rock:
        break;
      case fireball:
        break;
      case nothing:
        player->lightning->shoot =0;
        player->fire->shoot =0;
        player->wind->shoot = 0;
        player->water->shoot = 0;
        break;
    }
  }
  else
  {
    player->lightning->shoot =0;
    player->fire->shoot =0;
    player->wind->shoot = 0;
    player->water->shoot = 0;
  }



}

void gf2d_player_movement(TileMap * map)
{
  Vector2D velocity;
  cpVect v = cpBodyGetVelocity(player->ent->body);
  if(player->ent->colliding == 1 && (fabs(v.x) > 50 || fabs(v.y) > 50))
  {
    cpBodySetVelocity(player->ent->body,cpvmult(v,.9));
    return;
  }
  else if(player->ent->colliding == 1 && (fabs(v.x) <= 50 || fabs(v.y) <= 50))
  {
    player->ent->colliding = 0;
  }
  //slog("force is %f %f",force.x,force.y);

  if(gf2d_key_pressed(SDL_SCANCODE_LEFT))
  {
    if(gf2d_player_get_pos().x <=100 && gf2d_tilemap_get_gp()!=0)
    {
      velocity.x = 0;
      gf2d_player_set_x_velocity(velocity);
      gf2d_tilemap_move_left(map);
    }
    else if(gf2d_player_get_pos().x <=100 && gf2d_tilemap_get_gp()==0)
    {
      velocity.x = -150;
      gf2d_player_set_x_velocity(velocity);
    }
    else if(gf2d_player_get_pos().x >100)// && gf2d_tilemap_get_gp()!=0)
    {
      velocity.x = -150;
      gf2d_player_set_x_velocity(velocity);
    }
    else
    {
      velocity.x = 0;
      gf2d_player_set_x_velocity(velocity);
    }
    gf2d_player_set_left();
  }
  else if(gf2d_key_pressed(SDL_SCANCODE_RIGHT))
  {
    if(gf2d_player_get_pos().x >=1180 && gf2d_tilemap_get_gp()!=gf2d_tilemap_get_end_gp(map)*10)
    {
      //slog("right1");
      velocity.x = 0;
      gf2d_player_set_x_velocity(velocity);
      gf2d_tilemap_move_right(map);
    }
    else if(gf2d_player_get_pos().x >=1180 && gf2d_tilemap_get_gp()==gf2d_tilemap_get_end_gp(map)*10)
    {
      //slog("right2");
      velocity.x = 150;
      gf2d_player_set_x_velocity(velocity);
    }
    else if(gf2d_player_get_pos().x <1180)// && gf2d_tilemap_get_gp()!=gf2d_tilemap_get_end_gp(map))
    {
      //slog("right3");
      velocity.x = 150;
      gf2d_player_set_x_velocity(velocity);
    }
    else
    {
      velocity.x = 0;
      gf2d_player_set_x_velocity(velocity);
    }
    gf2d_player_set_right();
  }
  else
  {
    velocity.x = 0;
    gf2d_player_set_x_velocity(velocity);
  }

  if(gf2d_key_pressed(SDL_SCANCODE_UP))
  {
    velocity.y = -100;
    gf2d_player_set_y_velocity(velocity);
    gf2d_player_set_back();
  }
  else if(gf2d_key_pressed(SDL_SCANCODE_DOWN))
  {
    velocity.y = 100;
    gf2d_player_set_y_velocity(velocity);
    gf2d_player_set_forw();
  }
  else
  {
    velocity.y = 0;
    gf2d_player_set_y_velocity(velocity);
  }
}

Player * gf2d_player_get_player(int ind)
{
  return player;
}

void gf2d_player_set_invincible(int inv)
{
  player->invincible = inv;
}

cpShapeFilter gf2d_player_filter()
{
  cpGroup group;
  cpBitmask mask;
  cpBitmask cat;
  cpShapeFilter filter;
  group = 1;
  mask = ENEMIES;
  cat = PLAYER;
  filter = cpShapeFilterNew(group,cat,mask);
  return filter;
}

void gf2d_player_close()
{
  free(player);
}
