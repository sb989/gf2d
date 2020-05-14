#include "gf2d_level_editor.h"


typedef struct
{
  int  new_sprite_height;
  int new_sprite_width;
  int new_sprites_per_line;


  char * new_file;
  int load_new_file,save_new_file,create_new_hitbox;

  int new_hitbox_width,new_hitbox_height;
  int new_hitbox_posx,new_hitbox_posy;
  char * new_hitbox_name;

  char *save_file;
  Carousel * c;
  Grid *g;
}LevelEditorAssets;

static LevelEditorAssets lea ={0};
static enum layer editMode;



void gf2d_level_editor_init()
{
  gf2d_image_carousel_init();
  editMode = background;
  lea.new_file = NULL;
  lea.load_new_file = 0;
  lea.save_new_file = 0;
  lea.create_new_hitbox = 0;
  lea.new_hitbox_posx = lea.new_hitbox_posy = lea.new_hitbox_width = lea.new_hitbox_height = -1;
  lea.new_hitbox_name = NULL;
  gf2d_grid_init();

}

void gf2d_level_editor_load()
{
  slog("loading level_editor");
  int screen_width,screen_height;
  SDL_Window *win = gf2d_graphics_get_window();
  SDL_GetWindowSize(win,&screen_width,&screen_height);
  gf2d_ui_load("json_files/map_editor.json");
  lea.c = gf2d_image_carousel_init_carousel(screen_width,80,0,0,60);
  lea.g = gf2d_grid_init_grid(18,32,504,896,170,180);
}

Sprite * gf2d_level_editor_get_selected_sprite()
{
  if(lea.c && lea.c->selected && lea.c->selected->boxInfo)
    return lea.c->selected->boxInfo->s;
  else
    return NULL;
}

enum layer gf2d_level_editor_get_edit_mode()
{
  return editMode;
}

void gf2d_level_editor_toggle_edit_mode(void * button)
{
  ButtonInfo * butt = (ButtonInfo*) button;
  if(editMode == background)
  {
    editMode = foreground;
    butt->boxInfo->color = vector4d(255,0,80,255);
  }
  else
  {
    editMode = background;
    butt->boxInfo->color = vector4d(80,0,255,255);
  }
}

char * gf2d_level_editor_get_save_file()
{
  return lea.save_file;
}

int gf2d_level_editor_get_selected_sprite_num()
{
  if(lea.c && lea.c->selected && lea.c->selected->boxInfo)
    return lea.c->selected->boxInfo->sprite_num;
  else
    return 0;
}

void gf2d_level_editor_read_hitbox_params(void * nothing)
{
  char * name,*height,*width,*posx,*posy;

  name = gf2d_ui_typebox_get_text(3);
  height = gf2d_ui_typebox_get_text(4);
  width = gf2d_ui_typebox_get_text(5);
  posx = gf2d_ui_typebox_get_text(6);
  posy = gf2d_ui_typebox_get_text(7);


  if(strcmp(name," ") == 0 || strcmp(posx," ") == 0 || strcmp(posy," ") == 0 || strcmp(width," ") == 0 || strcmp(height," ")== 0 )
  {
    return;
  }

  lea.new_hitbox_name = name;
  lea.new_hitbox_posx = atoi(posx);
  lea.new_hitbox_posy = atoi(posy);
  lea.new_hitbox_width = atoi(width);
  lea.new_hitbox_height = atoi(height);

  lea.create_new_hitbox = 1;
}

void gf2d_level_editor_load_sprite(void * nothing)
{
  char * height, *width, *npl;

  slog("loading new file");

  height = gf2d_ui_typebox_get_text(0);
  width = gf2d_ui_typebox_get_text(1);
  npl = gf2d_ui_typebox_get_text(2);
  if(strcmp(height," ") == 0 || strcmp(width," ") == 0 || strcmp(npl," ") == 0 )
    return;
  lea.new_sprite_height = atoi(height);
  lea.new_sprite_width = atoi(width);
  lea.new_sprites_per_line = atoi(npl);
  slog(height,width,npl);
  lea.new_file = gf2d_gtk_get_filename();
  if(lea.new_file == NULL)
    return;
  //slog("new height:%d\tnew width:%d\tnew npl:%d\tnew file:%s",lea.new_sprite_height,lea.new_sprite_width,lea.new_sprites_per_line,lea.new_file);
  lea.load_new_file = 1;
}


void gf2d_level_editor_set_save_new_file(char * filename)
{
  lea.save_file = filename;
  lea.save_new_file = 1;
}

void gf2d_level_editor_update()
{
  int i,total_sprites;

  if(lea.load_new_file == 1)
  {
    //slog("updating level editor image carousel");
    Sprite *s = gf2d_sprite_load_all(lea.new_file,lea.new_sprite_width,lea.new_sprite_height,lea.new_sprites_per_line);
    total_sprites = (s->total_h/lea.new_sprite_width) * lea.new_sprites_per_line;
    slog("adding %d sprites to carousel",total_sprites);
    for(i=0;i<total_sprites;i++)
      gf2d_image_carousel_add_carousel_element(lea.c,s,i,NULL,&gf2d_image_carousel_set_selected);
    lea.load_new_file = 0;
  }
  if(lea.save_new_file == 1)
  {
    gf2d_grid_to_json_create_json_grids(gf2d_grid_return_all());
    lea.save_new_file = 0;
  }

  if(lea.create_new_hitbox == 1)
  {
    gf2d_grid_create_object_hitbox();
    lea.create_new_hitbox = 0;
  }

  if(SDL_GetTicks() - gf2d_grid_get_right_clicked() > 1 && gf2d_grid_get_right_clicked() != 0)
  {
    gf2d_grid_right_button();
  }
  if(SDL_GetTicks() - gf2d_grid_get_left_clicked() > 1 && gf2d_grid_get_left_clicked() != 0)
  {
    gf2d_grid_left_button();
  }
  if(lea.c)
  {
    //gf2d_image_carousel_reindex(lea.c);
    //slog("drawing carousel buttons");
    gf2d_image_carousel_draw_lr(lea.c);
  }
  gf2d_grid_draw();
}


int gf2d_level_editor_get_new_hitbox_posx()
{
  return lea.new_hitbox_posx;
}

int gf2d_level_editor_get_new_hitbox_posy()
{
  return lea.new_hitbox_posy;
}

int gf2d_level_editor_get_new_hitbox_height()
{
  return lea.new_hitbox_height;
}

int gf2d_level_editor_get_new_hitbox_width()
{
  return lea.new_hitbox_width;
}

char * gf2d_level_editor_get_new_hitbox_name()
{
  return lea.new_hitbox_name;
}


void gf2d_level_editor_close()
{
  slog("level editor cloise");
  gf2d_image_carousel_clear_list();
  gf2d_grid_clear_list();
  gf2d_ui_clear();
  lea.new_file = NULL;
  lea.load_new_file = 0;
  lea.save_new_file = 0;
  lea.c = NULL;
  lea.g = NULL;
}
