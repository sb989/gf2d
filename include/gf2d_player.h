#ifndef __GF2D_PLAYER_H_
#define __GF2D_PLAYER_H_
#include "gf2d_entity.h"
#include "gf2d_controls_state.h"
#include "gf2d_beam.h"

#include "gf2d_tilemap.h"

typedef struct
{
  Entity *ent;
  int forw,back,left,right;
  int frames_per_animation;
  float resizex,resizey,draw_height,draw_width;
  Beam *fire,*lightning,*water,*wind;
  int rock_limit,rock_count;
  int icicle_limit,icicle_count;
  int fireball_limit,fireball_count;
  int playerNum;
  int invincible;
}Player;

void gf2d_player_init();
Player * gf2d_player_new_player();
void gf2d_player_set_forw();
void gf2d_player_set_back();
void gf2d_player_set_left();
void gf2d_player_set_right();
void gf2d_player_animate(void *nothing);
cpShapeFilter gf2d_player_filter();
void gf2d_player_close();
void gf2d_player_update_position();
Vector2D gf2d_player_get_pos();
void gf2d_player_set_pos(Vector2D pos);
void gf2d_player_set_x_velocity(Vector2D velocity);
void gf2d_player_set_y_velocity(Vector2D velocity);
float gf2d_player_get_draw_height();
float gf2d_player_get_draw_width();
void gf2d_player_attack();
void gf2d_player_set_inuse(int inuse);
void gf2d_player_draw_bb();
Player * gf2d_player_get_player(int ind);
void gf2d_player_set_invincible(int inv);
void gf2d_player_set_attack();
#endif
