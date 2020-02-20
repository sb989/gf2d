#ifndef __GF2D_TILEMAP_H_
#define __GF2D_TILEMAP_H_
#include "gfc_vector.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
typedef struct
{
  Sprite * sprite;
  Vector2D pos;
  int dimx;
  int dimy;
  int frame;
  Uint8 _inuse;
}TileMap;


void gf2d_tilemap_init_manager(int resx,int resy);
TileMap * gf2d_tilemap_insert(Sprite * s,int frame,Vector2D pos);
TileMap * gf2d_tilemap_new(Sprite * s,int frame,Vector2D pos);
void gf2d_tilemap_draw_all();
void gf2d_tilemap_draw(int x);
void gf2d_tilemap_close();

#endif
