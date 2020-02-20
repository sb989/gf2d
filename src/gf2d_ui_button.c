#include "gf2d_ui_button.h"

typedef struct
{
  List *buttons;
}UIButtonAssets;

UIButtonAssets uba = {0};

void gf2d_ui_button_init()
{
  uba.buttons = gfc_list_new();
  atexit(gf2d_ui_button_close);
}

void gf2d_ui_button_load_buttons(SJson * buttons)
{
  int i,count;
  SJson * temp;
  count = sj_array_get_count(buttons);
  char * image_or_rect;
  BoxInfo * bi;
  ButtonInfo *tbi;
  for(i=0;i<count;i++)
  {
    temp = sj_array_get_nth(buttons,i);
    if(!temp) continue;
    tbi = gf2d_ui_button_new();
    image_or_rect = gf2d_ui_helper_functions_get_object_value_as_string(temp,"image_or_rect");
    if(strcmp(image_or_rect,"image")==0)
    {
      bi = gf2d_ui_box_info_image(temp);
      tbi->boxInfo = bi;
      gf2d_ui_button_sprite_init(temp,tbi);
      gf2d_ui_button_setup_collision_body(tbi);
    }
    else
    {
      gf2d_ui_box_info_rect(temp);
      tbi->boxInfo = bi;
      tbi->s1 = NULL;
      tbi->s2 = NULL;
      gf2d_ui_button_setup_collision_body(tbi);
    }


  }
}

void gf2d_ui_button_switch_box_sprite(ButtonInfo * bi)
{
  if(bi->boxInfo->s == bi->s1)
  {
    bi->boxInfo->s = bi->s2;
  }
  else
  {
    bi->boxInfo->s = bi->s1;
  }
}

void gf2d_ui_button_draw_all()
{
  gf2d_ui_box_draw_all();
}

void gf2d_ui_button_set_func_name(SJson * value,ButtonInfo *tbi)
{
  char * func_name;

  func_name = gf2d_ui_helper_functions_get_object_value_as_string(value,"func_name");
  tbi->func_name = func_name;
}

void gf2d_ui_button_sprite_init(SJson * value,ButtonInfo * tbi)
{
  int x,y,width,height;
  char * file_location,*xml_tagname,*json_location;
  Sprite * temp1,*temp2;
  SJson *tile_map_info,*sprite_info;

  file_location = gf2d_ui_helper_functions_get_object_value_as_string(value,"file_location");
  json_location = gf2d_ui_helper_functions_json_file_location(file_location);
  xml_tagname = gf2d_ui_helper_functions_get_object_value_as_string(value,"xml_tagname");
  tile_map_info = sj_load(json_location);
  sprite_info = gf2d_ui_box_get_sprite_info_from_json(tile_map_info,xml_tagname);



  x = atoi(gf2d_ui_helper_functions_get_object_value_as_string(sprite_info,"@x"));
  y = atoi(gf2d_ui_helper_functions_get_object_value_as_string(sprite_info,"@y"));
  width = atoi(gf2d_ui_helper_functions_get_object_value_as_string(sprite_info,"@width"));
  height = atoi(gf2d_ui_helper_functions_get_object_value_as_string(sprite_info,"@height"));
  temp1 = gf2d_sprite_load_all(file_location,x+width,y+height,1);

  file_location = gf2d_ui_helper_functions_get_object_value_as_string(value,"file_location2");
  x = atoi(gf2d_ui_helper_functions_get_object_value_as_string(sprite_info,"@x2"));
  y = atoi(gf2d_ui_helper_functions_get_object_value_as_string(sprite_info,"@y2"));
  width = atoi(gf2d_ui_helper_functions_get_object_value_as_string(sprite_info,"@width2"));
  height = atoi(gf2d_ui_helper_functions_get_object_value_as_string(sprite_info,"@height2"));
  temp2 = gf2d_sprite_load_all(file_location,x+width,y+height,1);

  tbi->s1 = temp1;
  tbi->s2 = temp2;
}

void gf2d_ui_button_setup_collision_body(ButtonInfo *b)
{
  Uint8 CollisionType = 0;
  cpFloat length,width,radius;

  length = b->boxInfo->height;
  width = b->boxInfo->width;
  radius = 0;
  b->shape = gf2d_physics_add_square_body(length,width,radius,1);
  b->body = cpShapeGetBody(b->shape);
  cpShapeSetCollisionType(b->shape,CollisionType);
  cpBodySetUserData(b->body,b);
}


ButtonInfo * gf2d_ui_button_new()
{

  ButtonInfo * temp = (ButtonInfo*) malloc(sizeof(ButtonInfo));

  temp->func_name = NULL;
  temp->boxInfo = gf2d_ui_box_new();
  temp->s1 = NULL;
  temp->s2 = NULL;

  temp->shape = NULL;
  temp->body = NULL;


  gfc_list_append(uba.buttons,temp);
  return temp;
}

void gf2d_ui_button_close()
{
  int count,i;
  ButtonInfo * temp;
  count = gfc_list_get_count(uba.buttons);
  for(i = 0;i<count;i++)
  {
    temp = gfc_list_get_nth(uba.buttons,i);
    free(temp->func_name);
    free(temp);
  }
}
