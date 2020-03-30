#include "gf2d_main_game.h"
#include "gf2d_enemy.h"
static TileMap * map;
static BoxInfo * b;
void gf2d_main_game_init()
{
  map = gf2d_tilemap_load_json("json_files/test5.json");
  gf2d_player_set_inuse(1);
  gf2d_enemy_init_enemy(vector2d(600,500),"red");
  gf2d_enemy_init_enemy(vector2d(600,350),"blue");
  gf2d_enemy_init_enemy(vector2d(500,400),"green");
  //gf2d_enemy_init_enemy(vector2d(340,40),"grey");
  gf2d_enemy_init_enemy(vector2d(700,400),"purple");
  b = gf2d_ui_box_init_box(NULL,0, 0,250,50,0,0,100,500,0,vector4d(255,255,255,255),0);

}

void gf2d_main_game_draw()
{
  gf2d_tilemap_draw(map);
  gf2d_enemy_draw_bb_all();
  //gf2d_player_animate();
}

void gf2d_main_game_set_box_color(Vector4D color)
{
  if(b)
  b->color = color;
}

void gf2d_main_game_update()
{

  gf2d_player_movement(map);

  gf2d_player_update_position();

  gf2d_main_game_draw();
  gf2d_beam_update_all();
  gf2d_player_attack();
}

void gf2d_main_game_close()
{
  gf2d_tilemap_clear();
}
