#include "gf2d_tilemap.h"

typedef struct
{
  TileMap ** tileArray;
  int xcount;
  int xsize;
  int resx,resy;
}TileMapManager;


TileMapManager mapManager = {0};

void gf2d_tilemap_init_manager(int resx,int resy)
{
  int x;
  mapManager.tileArray = (TileMap **) malloc(sizeof(TileMap*)*486);
  for(x = 0;x<486;x++)
  {
    mapManager.tileArray[x] = (TileMap*) malloc(sizeof(TileMap));
  }
  mapManager.xsize = 486;
  mapManager.xcount = 0;
  mapManager.resx = resx;
  mapManager.resy = resy;
}

TileMap * gf2d_tilemap_insert(Sprite * s,int frame,Vector2D pos)
{
  int x;
  x = mapManager.xcount;
  mapManager.tileArray[x]->sprite = s ;
  mapManager.tileArray[x]->frame = frame;
  mapManager.tileArray[x]->pos = pos;
  mapManager.tileArray[x]->_inuse = 1;

  mapManager.xcount = mapManager.xcount +1;
  return mapManager.tileArray[x];
}

TileMap * gf2d_tilemap_new(Sprite * s,int frame,Vector2D pos)
{
  if(mapManager.xcount == mapManager.xsize )
  {
    slog("tilemap full cannot create new tilemap");
    return NULL;
  }

  return gf2d_tilemap_insert(s,frame,pos);
}

void gf2d_tilemap_draw_all()
{
  int x;
  for(x=0; x<mapManager.xcount; x++)
  {
    gf2d_tilemap_draw(x);
    slog("x is %d",x);
  }
}


void gf2d_tilemap_draw(int x)
{
  TileMap * t = mapManager.tileArray[x];
  Vector2D scale = vector2d(40/(t->sprite->frame_w),40/(t->sprite->frame_h));
  gf2d_sprite_draw(t->sprite,t->pos,&scale,NULL,NULL,NULL,NULL,t->frame);

}
