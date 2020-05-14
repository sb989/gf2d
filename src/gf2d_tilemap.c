#include "gf2d_tilemap.h"

typedef struct
{
  List * tileMaps;
  List * hitbox;
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
  mapManager.hitbox = gfc_list_new();
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
  if(s == NULL)
  {
    map->tiles = gfc_list_append(map->tiles,temp);
    return temp;
  }
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
  SJson * map,*cells,*cell,*env,*envObject,*hitboxes,*hitbox;
  TileMap * tmap,*envmap;
  int tiles_per_row,num_rows;
  int i,count;
  map = sj_load(file_location);
  tiles_per_row = atoi(gf2d_ui_helper_functions_get_object_value_as_string(map,"tiles_per_row"));
  num_rows = atoi(gf2d_ui_helper_functions_get_object_value_as_string(map,"num_rows"));
  tmap = gf2d_tilemap_init_tilemap(tiles_per_row,num_rows);
  envmap = gf2d_tilemap_init_tilemap(tiles_per_row,num_rows);
  cells = sj_object_get_value(map,"cells");
  count = sj_array_get_count(cells);
  slog("cells count is %d",count);
  for(i=0;i<count;i++)
  {
    cell = sj_array_get_nth(cells,i);
    gf2d_tilemap_load_tile(cell,tmap);
  }
  env = sj_object_get_value(map,"env");
  count = sj_array_get_count(env);
  slog("env count is %d",count);
  for(i=0;i<count;i++)
  {
    envObject = sj_array_get_nth(env,i);
    gf2d_tilemap_load_tile(envObject,envmap);
  }
  hitboxes = sj_object_get_value(map,"hitboxes");
  count = sj_array_get_count(hitboxes);
  for(i=0;i<count;i++)
  {
    hitbox = sj_array_get_nth(hitboxes,i);
    gf2d_tilemap_load_hitbox(hitbox);
  }
  return tmap;
}

void gf2d_tilemap_load_hitbox(SJson* hitbox)
{
  int width,height,posx,posy;
  cpVect pos;

  posx = atoi(gf2d_ui_helper_functions_get_object_value_as_string(hitbox,"posx"));
  posy = atoi(gf2d_ui_helper_functions_get_object_value_as_string(hitbox,"posy"));
  width = atoi(gf2d_ui_helper_functions_get_object_value_as_string(hitbox,"width"));
  height = atoi(gf2d_ui_helper_functions_get_object_value_as_string(hitbox,"height"));

  pos.x = posx * mapManager.tile_width;
  pos.y = posy * mapManager.tile_height;
  height = height *mapManager.tile_height;
  width = width* mapManager.tile_width;
  cpShape * shape = gf2d_physics_add_square_body(width,height,0,0);
  cpBody * body = cpShapeGetBody(shape);
  cpShapeSetCollisionType(shape,OBJECTS);
  //cpBodySetUserData(body,);
  cpShapeSetFilter(shape,gf2d_tilemap_environment_filter());
  cpBodySetPosition(body,pos);
  cpSpaceReindexShapesForBody(gf2d_physics_get_space(),body);
  mapManager.hitbox = gfc_list_append(mapManager.hitbox,shape);
}


cpShapeFilter gf2d_tilemap_environment_filter()
{
  cpGroup group;
  cpBitmask mask;
  cpBitmask cat;
  cpShapeFilter filter;
  group = 3;
  mask = ENEMIES | PLAYER | ROCK | ICICLE | FIREBALL | COIN;
  cat = OBJECTS;
  filter = cpShapeFilterNew(group,cat,mask);
  return filter;
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

void gf2d_tilemap_draw()
{
  int i,j,tpr;
  Tile * tile,*env;
  float xoffset;
  TileMap * map = gfc_list_get_nth(mapManager.tileMaps,0);
  TileMap * envmap = gfc_list_get_nth(mapManager.tileMaps,1);
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
      if(tile->render == 1)
      {
        v2->x = tile->resizex;
        v2->y = tile->resizey;
        gf2d_sprite_draw(tile->sprite,vector2d(tile->pos.x-xoffset,tile->pos.y),v2,NULL,NULL,NULL,NULL,NULL,tile->frame);
      }
      if(envmap->tiles->count >0)
      {
        env = gfc_list_get_nth(envmap->tiles,j+(tpr*i));
        if(env->render == 1)
        {
          v2->x = env->resizex;
          v2->y = env->resizey;
          gf2d_sprite_draw(env->sprite,vector2d(env->pos.x-xoffset,env->pos.y),v2,NULL,NULL,NULL,NULL,NULL,env->frame);
        }
      }


    }
    for(j=1;j<5;j++)//draws 5 extra columns of tiles on each side
    {
      if((int)pos-j>=0)//draws tiles to the left of screen
      {
        tile = gfc_list_get_nth(map->tiles,(int)pos-j+(tpr*i));
        if(tile->render == 1)
        {
          v2->x = tile->resizex;
          v2->y = tile->resizey;
          gf2d_sprite_draw(tile->sprite,vector2d(tile->pos.x-xoffset,tile->pos.y),v2,NULL,NULL,NULL,NULL,NULL,tile->frame);
        }

        if(envmap->tiles->count >0)
        {
          env = gfc_list_get_nth(envmap->tiles,(int)pos-j+(tpr*i));
          if(env->render == 1)
          {
            v2->x = env->resizex;
            v2->y = env->resizey;
            gf2d_sprite_draw(env->sprite,vector2d(env->pos.x-xoffset,env->pos.y),v2,NULL,NULL,NULL,NULL,NULL,env->frame);
          }
        }
      }
      if((int)pos + mapManager.num_columns_on_screen+j-1 < tpr)//draws tiles to the right of screen
      {
        tile = gfc_list_get_nth(map->tiles,(int)pos + mapManager.num_columns_on_screen+j-1+(tpr*i));
        if(tile->render == 1)
        {
          v2->x = tile->resizex;
          v2->y = tile->resizey;
          gf2d_sprite_draw(tile->sprite,vector2d(tile->pos.x-xoffset,tile->pos.y),v2,NULL,NULL,NULL,NULL,NULL,tile->frame);
        }

        if(envmap->tiles->count >0)
        {
          env = gfc_list_get_nth(envmap->tiles,(int)pos + mapManager.num_columns_on_screen+j-1+(tpr*i));
          if(env->render == 1)
          {
            v2->x = env->resizex;
            v2->y = env->resizey;
            gf2d_sprite_draw(env->sprite,vector2d(env->pos.x-xoffset,env->pos.y),v2,NULL,NULL,NULL,NULL,NULL,env->frame);
          }
        }
      }
    }
  }
  free(v2);
}

void gf2d_tilemap_move_left(TileMap * map)
{
  int i,count;
  cpShape *shape;
  cpBody * body;
  cpVect pos;
  if(mapManager.gp == 0)
  {
    return;
  }
  else
  {
    mapManager.gp = mapManager.gp-1;
    count = gfc_list_get_count(mapManager.hitbox);
    for(i=0;i<count;i++)
    {
      shape = gfc_list_get_nth(mapManager.hitbox,i);
      body = cpShapeGetBody(shape);
      pos = cpBodyGetPosition(body);
      pos = cpvadd(cpv(0.1*mapManager.tile_width,0.0f),pos);
      cpBodySetPosition(body,pos);
    }
  }
  //slog("gp is %d",mapManager.gp);
}

void gf2d_tilemap_move_right(TileMap *map)
{
  int i ,count;
  cpShape *shape;
  cpBody * body;
  cpVect pos;

  if(mapManager.gp/10 == map->tiles_per_row-mapManager.num_columns_on_screen)
  {
    return;
  }
  else
  {
    mapManager.gp = mapManager.gp+1;
    count = gfc_list_get_count(mapManager.hitbox);
    for(i=0;i<count;i++)
    {
      shape = gfc_list_get_nth(mapManager.hitbox,i);
      body = cpShapeGetBody(shape);
      pos = cpBodyGetPosition(body);
      pos = cpvadd(cpv(-0.1*mapManager.tile_width,0.0f),pos);
      cpBodySetPosition(body,pos);
    }
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
  gf2d_tilemap_clear_all_hitboxes();
  gfc_list_delete(mapManager.hitbox);
  mapManager.hitbox = gfc_list_new();
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

void gf2d_tilemap_clear_all_hitboxes()
{
  int i,count;
  cpShape * shape;
  count = gfc_list_get_count(mapManager.hitbox);
  for(i=0;i<count;i++)
  {
    shape = gfc_list_get_nth(mapManager.hitbox,i);
    gf2d_tilemap_clear_hitbox(shape);
  }
}

void gf2d_tilemap_clear_hitbox(cpShape * shape)
{
  cpSpace * space = gf2d_physics_get_space();
  cpBody * body = cpShapeGetBody(shape);
  cpSpaceRemoveShape(space, shape);
  cpSpaceRemoveBody(space,body);
  cpShapeFree(shape);
  cpBodyFree(body);
}

int gf2d_tilemap_get_gp()
{
  return mapManager.gp;
}

void gf2d_tilemap_set_gp(int gp)
{
  mapManager.gp = gp;
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
  gf2d_tilemap_clear_all_hitboxes();
  gfc_list_delete(mapManager.hitbox);
  mapManager.hitbox = gfc_list_new();
  free(mapManager.tileMaps);
}
