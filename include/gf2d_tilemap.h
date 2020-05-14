#ifndef __GF2D_TILEMAP_H_
#define __GF2D_TILEMAP_H_
#include "gfc_vector.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "gf2d_grid.h"
#include "gf2d_ui_helper_functions.h"

typedef struct
{
  Sprite * sprite;
  Vector2D pos;// pos used for rendering changes as player moves
  int dimx,dimy;
  float resizex,resizey;
  int frame;
  int render;
  //Uint8 _inuse;
}Tile;

typedef struct
{
  List * tiles;
  int tiles_per_row;
  int num_rows;
}TileMap;

void gf2d_tilemap_init_manager(int c,int r,int resx,int resy);
TileMap * gf2d_tilemap_new_tilemap();
Tile * gf2d_tilemap_new_tile();

Tile * gf2d_tilemap_init_tile(Sprite * s,Vector2D pos,int frame,int render,TileMap *map);
TileMap * gf2d_tilemap_init_tilemap(int tiles_per_row,int num_rows);
TileMap * gf2d_tilemap_load_json(char * file_location);

void gf2d_tilemap_load_hitbox(SJson* hitbox);
void gf2d_tilemap_load_tile(SJson *cell,TileMap * map);
void gf2d_tilemap_draw();

void gf2d_tilemap_close();
void gf2d_tilemap_clear();
void gf2d_tilemap_clear_map(TileMap *map);
void gf2d_tilemap_clear_hitbox(cpShape * shape);
void gf2d_tilemap_clear_all_hitboxes();

void gf2d_tilemap_move_left(TileMap * map);
void gf2d_tilemap_move_right(TileMap *map);

int gf2d_tilemap_get_tile_width();
int gf2d_tilemap_get_tile_height();
int gf2d_tilemap_get_gp();
int gf2d_tilemap_get_end_gp(TileMap *map);
void gf2d_player_movement(TileMap * map);
void gf2d_tilemap_set_gp(int gp);

cpShapeFilter gf2d_tilemap_environment_filter();


#endif
