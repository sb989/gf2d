#include "gf2d_main_game.h"
#include "gf2d_enemy.h"
static TileMap * map;
static BoxInfo * b;
static int paused = 0;
static cpVect velocity_offset;
static List *levels;
static int currentLvl = 0;
static int nextLevel = 0;
static int xbound = 0;
void gf2d_main_game_init()
{
  levels = gfc_list_new();
  //map = gf2d_tilemap_load_json("json_files/test5.json");
  gf2d_main_game_load_levels_list("json_files/levels_list.json");
  gf2d_main_game_load_level(0);
  gf2d_player_set_inuse(1);
  gf2d_player_set_pos(vector2d(50,50));
  gf2d_player_set_hp(10);

  velocity_offset = cpv(0,0);

  gf2d_set_mouse_colliding(true);
  gf2d_main_game_init_ui();
}

void gf2d_main_game_load_levels_list(char *file_location)
{
  int i,count;
  SJson * list;
  SJString * lvl;
  list = sj_load(file_location);
  list = sj_object_get_value(list,"levels");
  count = sj_array_get_count(list);
  for(i = 0;i<count;i++)
  {
    lvl = sj_array_get_nth_as_string(list,i);
    levels = gfc_list_append(levels,lvl->text);
  }
}

void gf2d_main_game_load_level(int n)
{
  SJson * f;
  char * filename = gfc_list_get_nth(levels,n);
  f = sj_load(filename);
  char * map_name = gf2d_ui_helper_functions_get_object_value_as_string(f,"map_name");
  nextLevel = atoi(gf2d_ui_helper_functions_get_object_value_as_string(f,"next_level"));
  if(nextLevel == 1)
  {
    xbound = atoi(gf2d_ui_helper_functions_get_object_value_as_string(f,"xbound"));//distance from right edge before next level loads
  }
  else
  {
    xbound = 0;
  }
  currentLvl = n;
  map = gf2d_tilemap_load_json(map_name);
  gf2d_enemy_init_enemy(vector2d(500,400),"green");
  gf2d_enemy_init_enemy(vector2d(340,40),"grey");
  gf2d_enemy_init_enemy(vector2d(700,400),"purple");
  gf2d_enemy_init_enemy(vector2d(600,500),"red");
  gf2d_enemy_init_enemy(vector2d(600,350),"blue");
  gf2d_player_set_pos(vector2d(50,50));
  gf2d_tilemap_set_gp(0);
}

void gf2d_main_game_set_velocity_offset(cpVect vel)
{
  velocity_offset = vel;
}

cpVect gf2d_main_game_get_velocity_offset()
{
  return velocity_offset;
}

void gf2d_main_game_draw()
{
  gf2d_tilemap_draw(map);
  //gf2d_enemy_draw_bb_all();
  //gf2d_player_animate();
}

void gf2d_main_game_init_ui()
{
  b = gf2d_ui_box_init_box(NULL,0, 0,250,50,0,0,50,650,0,vector4d(255,0,0,255),0);
}

void gf2d_main_game_update_ui()
{
  if(b->width <= 0)
    return;
  b->width = (int)((float)((gf2d_player_get_hp()/10.0) *250.0));
  slog("width is %d",b->width);
}

void gf2d_main_game_set_box_color(Vector4D color)
{
  if(b)
  b->color = color;
}

void gf2d_main_game_update()
{

  if(gf2d_key_pressed(SDL_SCANCODE_RETURN) && paused!=1 && gf2d_game_state_get_state() !=5)
  {
    gf2d_game_state_set_pause_menu();
    gf2d_set_mouse_colliding(false);
    paused = 1;
  }
  if(paused == 1)
  {
    gf2d_main_game_draw();
    gf2d_player_set_x_velocity(0);
    gf2d_player_set_y_velocity(0);
  }
  else
  {
    gf2d_player_movement(map);

    gf2d_player_update_position();
    //slog("player pos is %f %f",gf2d_player_get_pos().x,gf2d_player_get_pos().y);
    gf2d_main_game_draw();
    gf2d_beam_update_all();
    gf2d_player_set_attack();
    gf2d_player_attack();
    if(nextLevel == 1)
    {
      if(gf2d_player_get_pos().x >=1280-xbound && gf2d_tilemap_get_gp()==gf2d_tilemap_get_end_gp(map)*10)
      {
        gf2d_main_game_load_level(currentLvl+1);
      }
    }
  }

}

void gf2d_main_game_set_paused(int p)
{
  paused = p;
  if(p == 0)
    gf2d_set_mouse_colliding(true);
}

int gf2d_main_game_get_paused()
{
  return paused;
}

void gf2d_main_game_close()
{
  gf2d_tilemap_clear();
  gf2d_enemy_clear();
  gf2d_player_set_inuse(0);
  gf2d_player_set_pos(vector2d(50,50));
}
