#ifndef __GF2D_LEVEL_EDITOR_
#define __GF2D_LEVEL_EDITOR_
#include <string.h>
#include "simple_logger.h"
#include "simple_json.h"
#include "simple_json_array.h"
#include "gfc_list.h"
#include "gf2d_sprite.h"

typedef struct
{
  Sprite * s;
  int xOffset;
  int yOffset;
  int width;
  int height;
  int resizex;
  int resizey;
}SpriteInfo;

  
void gf2d_level_editor_init();
void gf2d_level_editor_load_sprites(SJson *sprites);
char * gf2d_level_editor_get_object_value_as_string(SJson *object,char * key);
SJson * gf2d_level_editor_get_sprite_info(SJson *json_file,char * key);
char * gf2d_level_editor_remove_quotes(char * word);
char * gf2d_level_editor_sprite_json_file_location(char * file_location);


#endif
