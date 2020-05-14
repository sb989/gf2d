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
static float time_elap = 0;
static Vector2D dist_t;
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
  player->dist = 0.0f;
  player->lastUpdate = 0;
  player->ent->hp = 10;
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

void gf2d_player_take_damage(int dmg)
{
  player->ent->hp = player->ent->hp - dmg;
  gf2d_main_game_update_ui();
  if(player->ent->hp == 0)
    gf2d_game_state_set_game_over(NULL);
}

void gf2d_player_set_hp(int hp)
{
  player->ent->hp = hp;
}

int gf2d_player_get_hp()
{
  return player->ent->hp;
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

void gf2d_player_set_x_velocity(float x)
{
  cpVect pos;
  pos.x = x;
  pos.y = cpBodyGetVelocity(player->ent->body).y;
  cpBodySetVelocity(player->ent->body,pos);
}

void gf2d_player_set_y_velocity(float y)
{
  cpVect pos;
  pos.x = cpBodyGetVelocity(player->ent->body).x;
  pos.y = y;
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
  gf2d_controls_update();
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
  //cpVect e_offset;
  float y,x;
  Vector2D * normal;
  cpVect v = cpBodyGetVelocity(player->ent->body);
  Vector2D playerPos,mousePos;
  int speed = 250;
  float timeD;

  if(player->lastUpdate > 0)
    timeD = SDL_GetTicks() - player->lastUpdate;
  else
    timeD = 0;

  if(player->ent->colliding >0 && (fabs(v.x) > 50 || fabs(v.y) > 50))
  {
    cpBodySetVelocity(player->ent->body,cpvmult(v,.9/player->ent->colliding));
    return;
  }
  else if(player->ent->colliding >0 && (fabs(v.x) <= 50 || fabs(v.y) <= 50))
  {
    player->ent->colliding = 0;
    gf2d_player_set_x_velocity(0);
    gf2d_player_set_y_velocity(0);
    player->dir.x = 0;
    player->dir.y = 0;
    player->dist = 0;
    gf2d_main_game_set_velocity_offset(cpv(0,0));
  }

  gf2d_controls_update();

  if(gf2d_left_mouse_down())
  {
    playerPos = gf2d_player_get_pos();
    mousePos = gf2d_mouse_pos();
    //slog("playerpos is %f %f",playerPos.x,playerPos.y);
    if(vector2d_distance_between_less_than(playerPos,mousePos,10))
    {
      //gf2d_set_left_released(false);
      gf2d_player_set_x_velocity(0);
      gf2d_player_set_y_velocity(0);
      return;
    }
    player->dist = vector2d_magnitude_between(mousePos,playerPos);
    y = (mousePos.y - playerPos.y);
    x = (mousePos.x - playerPos.x);
    //slog("dist is %f",player->dist);

    time_elap = SDL_GetTicks();//clock();
    //slog("time is %f",time_elap/CLOCKS_PER_SEC);
    dist_t = playerPos;
    normal = (Vector2D*)malloc(sizeof(Vector2D));
    normal->x = x;
    normal->y = y;
    vector2d_normalize(normal);
    player->dir.x = normal->x;
    player->dir.y = normal->y;
    //slog("dir is %f %f",player->dir.x,player->dir.y);
    velocity.x = normal->x *speed;
    velocity.y = normal->y *speed;
    //slog("velocity is %f %f",velocity.x,velocity.y);
    gf2d_player_set_x_velocity(velocity.x);
    gf2d_player_set_y_velocity(velocity.y);
    if(fabs(velocity.x) > fabs(velocity.y) )
    {
      if(velocity.x > 0)
        gf2d_player_set_right();
      else if(velocity.x < 0)
        gf2d_player_set_left();
    }
    else
    {
      if(velocity.y > 0)
        gf2d_player_set_forw();
      else if(velocity.y <0)
        gf2d_player_set_back();
    }
    //gf2d_set_left_released(false);
    timeD = 0;
  }
  Vector2D d;
  vector2d_scale(d,player->dir,speed*1.6);
  vector2d_scale(d,d,timeD/1000);
  player->dist = player->dist - vector2d_magnitude(d);
  //slog("magnitude of d is %f",vector2d_magnitude(d));
  if(player->dist <=0)
  {
    if(player->dir.x > 0 || player->dir.y>0)
    {
      vector2d_sub(dist_t,gf2d_player_get_pos(),dist_t);
      //slog("playerPos is %f %f",gf2d_player_get_pos().x,gf2d_player_get_pos().y);
      //slog("change in distance is %f",vector2d_magnitude(dist_t));
      //slog("change in time is %f",(SDL_GetTicks()-time_elap)/1000);
    }
    gf2d_player_set_x_velocity(0);
    gf2d_player_set_y_velocity(0);
    player->dir.x = 0;
    player->dir.y = 0;
    player->dist = 0;
    gf2d_main_game_set_velocity_offset(cpv(0,0));
    return;
  }
  if(player->dir.x < 0)
  {
    if(gf2d_player_get_pos().x <= 420 && gf2d_tilemap_get_gp()!=0 )
    {
      gf2d_player_set_x_velocity(0);
      gf2d_tilemap_move_left(map);
      gf2d_main_game_set_velocity_offset(cpv(250,0));
    }
    else if(gf2d_player_get_pos().x <= 420 && gf2d_tilemap_get_gp() == 0)
    {
      gf2d_player_set_x_velocity(speed*player->dir.x);
      gf2d_main_game_set_velocity_offset(cpv(0,0));
    }
    else if(gf2d_player_get_pos().x > 420)
    {
      gf2d_player_set_x_velocity(speed*player->dir.x);
      gf2d_main_game_set_velocity_offset(cpv(0,0));
    }
    else
    {
      gf2d_player_set_x_velocity(0);
      gf2d_main_game_set_velocity_offset(cpv(0,0));
    }
  }
  else if(player->dir.x > 0)
  {
    if(gf2d_player_get_pos().x >=860 && gf2d_tilemap_get_gp()!=gf2d_tilemap_get_end_gp(map)*10)
    {
      gf2d_player_set_x_velocity(0);
      gf2d_tilemap_move_right(map);
      gf2d_main_game_set_velocity_offset(cpv(-250,0));
    }
    else if(gf2d_player_get_pos().x >=860 && gf2d_tilemap_get_gp()==gf2d_tilemap_get_end_gp(map)*10)
    {
      gf2d_player_set_x_velocity(speed*player->dir.x);
      gf2d_main_game_set_velocity_offset(cpv(0,0));
    }
    else if(gf2d_player_get_pos().x <860)
    {
      gf2d_player_set_x_velocity(speed*player->dir.x);
      gf2d_main_game_set_velocity_offset(cpv(0,0));
    }
    else
    {
      gf2d_player_set_x_velocity(0);
      gf2d_main_game_set_velocity_offset(cpv(0,0));
    }
  }


  player->lastUpdate =SDL_GetTicks();

  //slog("the x velocity is %f",cpBodyGetVelocity(player->ent->body).x);
  //slog("player->dist is %f",player->dist);
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
  mask = ENEMIES|COIN|OBJECTS;
  cat = PLAYER;
  filter = cpShapeFilterNew(group,cat,mask);
  return filter;
}

void gf2d_player_close()
{
  free(player);
}
