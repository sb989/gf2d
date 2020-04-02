#include "gf2d_tilemap.h"

typedef struct
{
  List * tileMaps;
  int num_columns_on_screen;
  int num_rows_on_screen;
  int resx,resy;
  int tile_width,tile_height;
  Vector2D global_pos; //current position of the upper and leftmost tile in the entire map <---never used
  int gp;// the index of the upper and leftmost tile on screen(has to be dvided by ten to get the actual value)
}TileMapManager;


static TileMapManager mapManager = {0};

int gf2d_tilemap_get_tile_width()
{
  return mapManager.tile_width;
}

int gf2d_tilemap_get_tile_height()
{
  return mapManager.tile_height;
}

void gf2d_tilemap_init_manager(int c,int r,int resx,int resy)
{
  mapManager.tileMaps = gfc_list_new();
  mapManager.num_rows_on_screen = r;
  mapManager.num_columns_on_screen = c;
  mapManager.global_pos.x = 0;
  mapManager.global_pos.y = 0;
  mapManager.gp = 0;
  mapManager.resx =resx;
  mapManager.resy = resy;
  mapManager.tile_width = resx/c;
  mapManager.tile_height = resy/r;
  atexit(gf2d_tilemap_close);
}

TileMap * gf2d_tilemap_new_tilemap()
{
  TileMap * temp = (TileMap*) malloc(sizeof(TileMap));
  temp->tiles = gfc_list_new();
  mapManager.tileMaps = gfc_list_append(mapManager.tileMaps,temp);
  return temp;
}

Tile * gf2d_tilemap_new_tile()
{
  Tile * temp = (Tile*) malloc(sizeof(Tile));
  return temp;
}

Tile * gf2d_tilemap_init_tile(Sprite * s,Vector2D pos,int frame,int render,TileMap *map)
{
  Tile * temp = gf2d_tilemap_new_tile();
  temp->sprite = s;
  temp->pos = pos;
  temp->frame = frame;
  temp->render = render;
  temp->dimx = mapManager.tile_width;
  temp->dimy = mapManager.tile_height;
  temp->resizex = (float)temp->dimx/(float)(temp->sprite->frame_w);
  temp->resizey = (float)temp->dimy/(float)(temp->sprite->frame_h);
  map->tiles = gfc_list_append(map->tiles,temp);
  return temp;
}

int gf2d_tilemap_get_end_gp(TileMap *map)
{
  int egp = map->tiles_per_row - mapManager.num_columns_on_screen;
  //slog("egp is %d",egp);
  return egp;
}

TileMap * gf2d_tilemap_init_tilemap(int tiles_per_row,int num_rows)
{
  TileMap* temp = gf2d_tilemap_new_tilemap();
  temp->tiles_per_row = tiles_per_row;
  temp->num_rows = num_rows;
  return temp;
}

TileMap * gf2d_tilemap_load_json(char * file_location)
{
  SJson * map,*cells,*cell;
  TileMap * tmap;
  int tiles_per_row,num_rows;
  int i,count;
  map = sj_load(file_location);
  tiles_per_row = atoi(gf2d_ui_helper_functions_get_object_value_as_string(map,"tiles_per_row"));
  num_rows = atoi(gf2d_ui_helper_functions_get_object_value_as_string(map,"num_rows"));
  tmap = gf2d_tilemap_init_tilemap(tiles_per_row,num_rows);
  cells = sj_object_get_value(map,"cells");
  count = sj_array_get_count(cells);
  for(i=0;i<count;i++)
  {
    cell = sj_array_get_nth(cells,i);
    gf2d_tilemap_load_tile(cell,tmap);
  }
  return tmap;
}

void gf2d_tilemap_load_tile(SJson *cell,TileMap * map)
{
  Sprite *s;
  Vector2D pos;
  int frame,render,frames_per_line,width,height;
  int sprite_height,sprite_width;
  float resizex,resizey;
  //Tile * tile;
  char * empty_cell,*file_location;
  empty_cell = gf2d_ui_helper_functions_get_object_value_as_string(cell,"empty cell?");
  //slog("empty cell is %s",empty_cell);
  pos.x = atoi(gf2d_ui_helper_functions_get_object_value_as_string(cell,"relative x position"));
  pos.x = pos.x * mapManager.tile_width;
  pos.y = atoi(gf2d_ui_helper_functions_get_object_value_as_string(cell,"relative y position"));
  pos.y = pos.y * mapManager.tile_height;
  if(strcmp(empty_cell,"yes")==0)
  {
    frame = 0;
    render = 0;
    s = NULL;
    gf2d_tilemap_init_tile(s,pos,frame,render,map);
  }
  else
  {
    width = atoi(gf2d_ui_helper_functions_get_object_value_as_string(cell,"width"));
    height = atoi(gf2d_ui_helper_functions_get_object_value_as_string(cell,"height"));
    resizex = atof(gf2d_ui_helper_functions_get_object_value_as_string(cell,"resizex"));
    resizey = atof(gf2d_ui_helper_functions_get_object_value_as_string(cell,"resizey"));
    frame = atoi(gf2d_ui_helper_functions_get_object_value_as_string(cell,"sprite_num"));
    file_location = gf2d_ui_helper_functions_get_object_value_as_string(cell,"file_location");
    frames_per_line = atoi(gf2d_ui_helper_functions_get_object_value_as_string(cell,"frames_per_line"));
    sprite_width = (float)(width)/resizex;
    sprite_height = (float)(height)/resizey;
    render = 1;
    s = gf2d_sprite_load_all(file_location,sprite_width,sprite_height,frames_per_line);
    gf2d_tilemap_init_tile(s,pos,frame,render,map);
  }

}

void gf2d_tilemap_draw(TileMap *map)
{
  int i,j,tpr;
  Tile * tile;
  float xoffset;
  //float resizex,resizey;
  Vector2D *v2;
  float pos = ((float)mapManager.gp)/10;
  tpr = map->tiles_per_row;
  v2 = (Vector2D * )malloc(sizeof(Vector2D));
  //slog("num of rows is %d",mapManager.num_rows_on_screen);
  //slog("num of columns is %d",mapManager.num_columns_on_screen);
  //slog("gp is %f",mapManager.gp);
  xoffset = pos * (float)mapManager.tile_width;
  for(i=0;i<mapManager.num_rows_on_screen;i++)
  {
    for(j=(int)(pos);j<mapManager.num_columns_on_screen+(int)(pos);j++)
    {
      tile = gfc_list_get_nth(map->tiles,j+(tpr*i));
      v2->x = tile->resizex;
      v2->y = tile->resizey;
      gf2d_sprite_draw(tile->sprite,vector2d(tile->pos.x-xoffset,tile->pos.y),v2,NULL,NULL,NULL,NULL,NULL,tile->frame);
    }
    for(j=1;j<5;j++)
    {
      if((int)pos-j>=0)
      {
        tile = gfc_list_get_nth(map->tiles,(int)pos-j+(tpr*i));
        v2->x = tile->resizex;
        v2->y = tile->resizey;
        gf2d_sprite_draw(tile->sprite,vector2d(tile->pos.x-xoffset,tile->pos.y),v2,NULL,NULL,NULL,NULL,NULL,tile->frame);
      }
      if((int)pos + mapManager.num_columns_on_screen+j-1 < tpr)
      {
        tile = gfc_list_get_nth(map->tiles,(int)pos + mapManager.num_columns_on_screen+j-1+(tpr*i));
        v2->x = tile->resizex;
        v2->y = tile->resizey;
        gf2d_sprite_draw(tile->sprite,vector2d(tile->pos.x-xoffset,tile->pos.y),v2,NULL,NULL,NULL,NULL,NULL,tile->frame);
      }
    }
  }
  free(v2);
}

void gf2d_tilemap_move_left(TileMap * map)
{

  if(mapManager.gp == 0)
  {
    return;
  }
  else
  {
    mapManager.gp = mapManager.gp-1;
  }
  //slog("gp is %d",mapManager.gp);
}

void gf2d_tilemap_move_right(TileMap *map)
{

  if(mapManager.gp/10 == map->tiles_per_row-mapManager.num_columns_on_screen)
  {
    return;
  }
  else
  {
    mapManager.gp = mapManager.gp+1;
  }
  //slog("gp is %d",mapManager.gp);
}

void gf2d_tilemap_clear()
{
  int i,count;
  TileMap * map;
  count = gfc_list_get_count(mapManager.tileMaps);
  for(i = 0;i<count;i++)
  {
      map = gfc_list_get_nth(mapManager.tileMaps,i);
      gf2d_tilemap_clear_map(map);
      free(map);
  }
  free(mapManager.tileMaps);
  mapManager.global_pos.x = 0;
  mapManager.global_pos.y = 0;
  mapManager.gp = 0;
  mapManager.tileMaps = gfc_list_new();
}

void gf2d_tilemap_clear_map(TileMap *map)
{
  int i,count;
  count = gfc_list_get_count(map->tiles);
  Tile * t;
  for(i =0;i<count;i++)
  {
    t = gfc_list_get_nth(map->tiles,i);
    free(t);
  }
}

int gf2d_tilemap_get_gp()
{
  return mapManager.gp;
}

void gf2d_tilemap_close()
{
  int i,count;
  TileMap * map;
  count = gfc_list_get_count(mapManager.tileMaps);
  for(i = 0;i<count;i++)
  {
      map = gfc_list_get_nth(mapManager.tileMaps,i);
      gf2d_tilemap_clear_map(map);
      free(map);
  }
  free(mapManager.tileMaps);
}
