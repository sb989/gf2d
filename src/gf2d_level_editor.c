#include "gf2d_level_editor.h"


typedef struct
{
  List * sprites;
}LevelEditorAssets;



static LevelEditorAssets lea = {0};

void gf2d_level_editor_init()
{
  SJson *level,*sprites;
  level = sj_load("json_files/map_editor.json");
  if(!level)
  {
    slog("failed to load json file");
    return;
  }
  lea.sprites = gfc_list_new();
  sprites = sj_object_new();
  sprites = sj_object_get_value(level,"sprites");
  gf2d_level_editor_load_sprites(sprites);

}


void gf2d_level_editor_load_sprites(SJson *sprites)
{
  int count,i,x,y,width,height;
  SJson *value,*tile_map_info,*sprite_info;
  Sprite * temp;
  char *file_location,*posx,*posy,*xml_tagname,*resize,*resizex,*resizey,*json_location;

  count = sj_array_get_count(sprites);
  for(i=0;i<count;i++)
  {
    value = sj_array_get_nth(sprites,i);
    if(!value)continue;
    file_location = gf2d_level_editor_get_object_value_as_string(value,"file_location");
    posx = gf2d_level_editor_get_object_value_as_string(value,"posx");
    posy = gf2d_level_editor_get_object_value_as_string(value,"posy");
    xml_tagname = gf2d_level_editor_get_object_value_as_string(value,"xml_tagname");
    resize = gf2d_level_editor_get_object_value_as_string(value,"resize");
    json_location = gf2d_level_editor_sprite_json_file_location(file_location);
    tile_map_info = sj_load(json_location);
    sprite_info = gf2d_level_editor_get_sprite_info(tile_map_info,xml_tagname);
    x = atoi(gf2d_level_editor_get_object_value_as_string(sprite_info,"@x"));
    y = atoi(gf2d_level_editor_get_object_value_as_string(sprite_info,"@y"));
    width = atoi(gf2d_level_editor_get_object_value_as_string(sprite_info,"@width"));
    height = atoi(gf2d_level_editor_get_object_value_as_string(sprite_info,"@height"));

    temp = gf2d_sprite_load_all(file_location,x+width,y+height,1);
    SpriteInfo * temp_si = (SpriteInfo*) malloc(sizeof(SpriteInfo));
    temp_si->s = temp;
    temp_si->xOffset = x;
    temp_si->yOffset = y;
    temp_si->width = width;
    temp_si->height = height;
    if(strcmp(resize,"yes")==0)
    {
      resizex = gf2d_level_editor_get_object_value_as_string(value,"resizex");
      resizey = gf2d_level_editor_get_object_value_as_string(value,"resizey");
      temp_si->resizex = atoi(resizex);
      temp_si->resizey = atoi(resizey);
    }
    gfc_list_append(lea.sprites,temp);
  }
}

char * gf2d_level_editor_get_object_value_as_string(SJson *object,char * key)
{
  SJString *output;
  SJson *temp_value;
  char * temp;
  temp_value = sj_object_get_value(object,key);
  //sj_echo(temp_value);
  output = temp_value->get_string(temp_value);
  temp = output->text;
  temp = gf2d_level_editor_remove_quotes(temp);
  sj_string_free(output);
  return temp;
}

SJson * gf2d_level_editor_get_sprite_info(SJson *json_file,char * key)
{
  int i,count;
  SJson * temp,*object;
  char * name;
  object = sj_object_get_value(json_file,"SubTexture");
  count = sj_array_get_count(object);
  //sj_echo(object);
  for(i = 0;i<count;i++)
  {
    temp = sj_array_get_nth(object,i);
    //sj_echo(temp);
    name = gf2d_level_editor_get_object_value_as_string(temp,"@name");
    if(strcmp(name,key)==0)
      return temp;

  }
  return NULL;
}

char * gf2d_level_editor_remove_quotes(char * word)
{
  char * temp;
  int i;
  temp = malloc(sizeof(char)*strlen(word));
  for(i = 0;i<strlen(word)-2;i++)
  {
    temp[i] = word[i+1];
  }
  temp[i] = '\0';
  return temp;
}

char * gf2d_level_editor_sprite_json_file_location(char * file_location)
{
  char *temp;
  temp = malloc(sizeof(char)*(strlen(file_location)+2));
  strncpy(temp,file_location,strlen(file_location)-3);
  temp[strlen(file_location)-3] = 'j';
  temp[strlen(file_location)-2] = 's';
  temp[strlen(file_location)-1] = 'o';
  temp[strlen(file_location)] = 'n';
  temp[strlen(file_location)+1] = '\0';
  return temp;
}
