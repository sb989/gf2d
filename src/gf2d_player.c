#include "gf2d_player.h"
#include "gf2d_fire.h"
#include "gf2d_lightning.h"
#include "gf2d_rock.h"

static Player *player;
enum directions {left,right,back,forw};
static enum directions direction;
static int playerCount = 0;
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
  player->ent->_inuse = 0;
  player->forw = 24;
  player->back = 25;
  player->left = (23);
  player->right = (26);
  player->frames_per_animation = 3;
  player->ent->frame = 0;

  player->draw_width = 1.5*gf2d_tilemap_get_tile_width();
  player->draw_height = 1.5*gf2d_tilemap_get_tile_height();

  player->fire = gf2d_fire_init();
  slog("finished with player fire");
  player->lightning = gf2d_lightning_init();
  slog("finished with player lightning ");
  player->rock_limit = 1;
  player->rock_count = 0;
  direction = forw;
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
  if(player->ent->_inuse == 0)
    return;
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

  gf2d_sprite_draw(player->ent->s,player->ent->position,v2,NULL,NULL,NULL,NULL,NULL,f);
  free(v2);
  gf2d_player_draw_bb();
  if(player->ent->frame > player->frames_per_animation-1)
    player->ent->frame = 0;
  else if(player->ent->frame <= player->frames_per_animation-1)
    player->ent->frame = player->ent->frame+.1;
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

void gf2d_player_attack()
{
  if(gf2d_key_pressed(SDL_SCANCODE_R))
  {
    //slog("rrr");
    player->fire->shoot = 1;
    player->lightning->shoot =0;
    //gf2d_beam_animate(player->fire);
    //gf2d_fire_animate();
  }
  else if(gf2d_key_pressed(SDL_SCANCODE_T))
  {
    //slog("lightning");
    player->lightning->shoot = 1;
    player->fire->shoot =0;
    //gf2d_lightning_animate();
    //gf2d_beam_animate(player->lightning);
  }
  else if(gf2d_key_pressed(SDL_SCANCODE_E))
  {

    if(player->rock_count<player->rock_limit)
    {
      slog("rock count is %d",player->rock_count);
      player->rock_count +=1;
      slog("rock count is %d",player->rock_count);
      gf2d_rock_shoot(player->playerNum);
    }
    //gf2d_rock_animate();
  }
  else
  {
    player->lightning->shoot =0;
    player->fire->shoot =0;
  }
}

void gf2d_player_movement(TileMap * map)
{
  Vector2D velocity;

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

cpShapeFilter gf2d_player_filter()
{
  cpGroup group;
  cpBitmask mask;
  cpBitmask cat;
  cpShapeFilter filter;
  group = 0;
  mask = ENEMIES;
  cat = PLAYER;
  filter = cpShapeFilterNew(group,cat,mask);
  return filter;
}

void gf2d_player_close()
{
  free(player);
}
