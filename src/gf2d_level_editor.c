#include "gf2d_level_editor.h"


typedef struct
{
  char * new_file;
  int  new_sprite_height;
  int new_sprite_width;
  int new_sprites_per_line;
  int load_new_file;

  Carousel * c;
  Grid *g;
}LevelEditorAssets;

static LevelEditorAssets lea ={0};



void gf2d_level_editor_init()
{
  int screen_width,screen_height;
  gf2d_ui_load("json_files/map_editor.json");
  gf2d_image_carousel_init();
  SDL_Window *win = gf2d_graphics_get_window();
  SDL_GetWindowSize(win,&screen_width,&screen_height);
  lea.c = gf2d_image_carousel_init_carousel(screen_width,80,0,0,60);
  lea.new_file = NULL;
  lea.load_new_file = 0;

  gf2d_grid_init();
  lea.g = gf2d_grid_init_grid(18,27,504,756,150,200);
}

Sprite * gf2d_level_editor_get_selected_sprite()
{
  if(lea.c && lea.c->selected && lea.c->selected->boxInfo)
    return lea.c->selected->boxInfo->s;
  else
    return NULL;
}

int gf2d_level_editor_get_selected_sprite_num()
{
  if(lea.c && lea.c->selected && lea.c->selected->boxInfo)
    return lea.c->selected->boxInfo->sprite_num;
  else
    return 0;
}

void gf2d_level_editor_load_sprite(void * nothing)
{
  TypeBox * height, *width, *npl;
  slog("loading new file");
  height = gf2d_ui_typebox_get_typebox(0);
  width = gf2d_ui_typebox_get_typebox(1);
  npl = gf2d_ui_typebox_get_typebox(2);

  lea.new_sprite_height = atoi(height->text);
  lea.new_sprite_width = atoi(width->text);
  lea.new_sprites_per_line = atoi(npl->text);
  lea.new_file = gf2d_gtk_get_filename();
  slog("new height:%d\tnew width:%d\tnew npl:%d\tnew file:%s",lea.new_sprite_height,lea.new_sprite_width,lea.new_sprites_per_line,lea.new_file);
  lea.load_new_file = 1;
}



void gf2d_level_editor_update()
{
  int i,total_sprites;

  if(lea.load_new_file == 1)
  {
    slog("updating level editor image carousel");
    Sprite *s = gf2d_sprite_load_all(lea.new_file,lea.new_sprite_width,lea.new_sprite_height,lea.new_sprites_per_line);
    total_sprites = (s->total_h/lea.new_sprite_width) * lea.new_sprites_per_line;
    for(i=0;i<total_sprites;i++)
      gf2d_image_carousel_add_carousel_element(lea.c,s,i,NULL,&gf2d_image_carousel_set_selected);
    lea.load_new_file = 0;
  }
/*  if(lea.c->moving == 1)
  {
    gf2d_image_carousel_update_element_position_all(lea.c);
  }
  */
  //gf2d_image_carousel_update_element_visibility_all(lea.c);
  gf2d_image_carousel_draw_lr(lea.c);
  gf2d_grid_draw(lea.g);
}
