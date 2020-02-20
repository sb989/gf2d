#include "gf2d_ui_box.h"
#include "gf2d_draw.h"

typedef struct
{
  List * boxes;
}UIBoxAssets;

static UIBoxAssets uba = {0};

void gf2d_ui_box_init()
{

  uba.boxes = gfc_list_new();
  atexit(gf2d_ui_box_close);
}

BoxInfo * gf2d_ui_box_new()
{
  BoxInfo * temp = (BoxInfo*) malloc(sizeof(BoxInfo));
  temp->s = NULL;
  temp->xOffset = 0;
  temp->yOffset = 0;
  temp->width = 0;
  temp->height = 0;
  temp->resizex = 1;
  temp->resizey = 1;
  temp->posx = 0;
  temp->posy = 0;
  temp->image_or_rect = 0;
  gfc_list_append(uba.boxes,temp);
  return temp;
}

void gf2d_ui_box_load_boxes(SJson *boxes)
{
  int count,i;
  SJson *value;
  char *image_or_rect;

  count = sj_array_get_count(boxes);
  for(i=0;i<count;i++)
  {
    value = sj_array_get_nth(boxes,i);
    if(!value)continue;
    image_or_rect = gf2d_ui_helper_functions_get_object_value_as_string(value,"image_or_rect");
    if(strcmp(image_or_rect,"image")==0)
    {
      gf2d_ui_box_info_image(value);
      //gfc_list_append(uba.boxes,temp_si);
    }
    else
    {
      gf2d_ui_box_info_rect(value);
      //gfc_list_append(uba.boxes,temp_si);
    }
  }
}



SJson * gf2d_ui_box_get_sprite_info_from_json(SJson *json_file,char * key)
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
    name = gf2d_ui_helper_functions_get_object_value_as_string(temp,"@name");
    if(strcmp(name,key)==0)
      return temp;

  }
  return NULL;
}


 BoxInfo * gf2d_ui_box_info_image(SJson * value)
{
  char *file_location,*posx,*posy,*xml_tagname;
  char *resize,*resizex,*resizey,*json_location;
  int x,y,width,height;
  int screen_width,screen_height;
  SJson *tile_map_info,*sprite_info;
  Sprite * temp;
  BoxInfo * temp_si;

  file_location = gf2d_ui_helper_functions_get_object_value_as_string(value,"file_location");
  posx = gf2d_ui_helper_functions_get_object_value_as_string(value,"posx");
  posy = gf2d_ui_helper_functions_get_object_value_as_string(value,"posy");
  xml_tagname = gf2d_ui_helper_functions_get_object_value_as_string(value,"xml_tagname");
  resize = gf2d_ui_helper_functions_get_object_value_as_string(value,"resize");
  json_location = gf2d_ui_helper_functions_json_file_location(file_location);
  tile_map_info = sj_load(json_location);
  sprite_info = gf2d_ui_box_get_sprite_info_from_json(tile_map_info,xml_tagname);
  x = atoi(gf2d_ui_helper_functions_get_object_value_as_string(sprite_info,"@x"));
  y = atoi(gf2d_ui_helper_functions_get_object_value_as_string(sprite_info,"@y"));
  width = atoi(gf2d_ui_helper_functions_get_object_value_as_string(sprite_info,"@width"));
  height = atoi(gf2d_ui_helper_functions_get_object_value_as_string(sprite_info,"@height"));

  temp = gf2d_sprite_load_all(file_location,x+width,y+height,1);
  temp_si = gf2d_ui_box_new();
  temp_si->s = temp;
  temp_si->xOffset = x;
  temp_si->yOffset = y;
  temp_si->width = width;
  temp_si->height = height;
  temp_si->posx = atoi(posx);
  temp_si->posy = atoi(posy);
  temp_si->image_or_rect = 1;
  //sj_echo(value);
  if(strcmp(resize,"yes")==0)
  {

    resizex = gf2d_ui_helper_functions_get_object_value_as_string(value,"resize_x");
    resizey = gf2d_ui_helper_functions_get_object_value_as_string(value,"resize_y");
    SDL_Window *win = gf2d_graphics_get_window();
    SDL_GetWindowSize(win,&screen_width,&screen_height);
    if(strcmp(resizex,"screen_width")==0)
    {
      temp_si->resizex = screen_width;
    }
    else
    {
      temp_si->resizex = atoi(resizex);
    }
    if(strcmp(resizey,"screen_height")==0)
    {
      temp_si->resizey = screen_height;
    }
    else
    {
      temp_si->resizey = atoi(resizey);
    }
  }
  free(json_location);
  return temp_si;
}

BoxInfo * gf2d_ui_box_info_rect(SJson * value)
{

  char *posx,*posy;
  int screen_width,screen_height;
  BoxInfo * temp_si;
  SDL_Window *win;
  Vector4D rcolor;

  posx = gf2d_ui_helper_functions_get_object_value_as_string(value,"posx");
  posy = gf2d_ui_helper_functions_get_object_value_as_string(value,"posy");
  win = gf2d_graphics_get_window();
  SDL_GetWindowSize(win,&screen_width,&screen_height);
  temp_si = gf2d_ui_box_new();
  if(strcmp(gf2d_ui_helper_functions_get_object_value_as_string(value,"width"),"screen_width")==0)
  {
    temp_si->width = screen_width;
  }
  else
  {
    temp_si->width = atoi(gf2d_ui_helper_functions_get_object_value_as_string(value,"width"));
  }
  if(strcmp(gf2d_ui_helper_functions_get_object_value_as_string(value,"height"),"screen_height")==0)
  {
    temp_si->height = screen_height;
  }
  else
  {
    temp_si->height = atoi(gf2d_ui_helper_functions_get_object_value_as_string(value,"height"));
  }
  rcolor.x = atoi(gf2d_ui_helper_functions_get_object_value_as_string(value,"color.x"));
  rcolor.y = atoi(gf2d_ui_helper_functions_get_object_value_as_string(value,"color.y"));
  rcolor.z = atoi(gf2d_ui_helper_functions_get_object_value_as_string(value,"color.z"));
  rcolor.w = atoi(gf2d_ui_helper_functions_get_object_value_as_string(value,"color.w"));
  slog("x is %f y is %f w is %f h is %f",rcolor.x,rcolor.y,rcolor.z,rcolor.w);
  temp_si->color = rcolor;

  temp_si->posx = atoi(posx);
  temp_si->posy = atoi(posy);
  return temp_si;
}

void gf2d_ui_box_draw_all()
{
  int count,i;
  BoxInfo *s;

  count = gfc_list_get_count(uba.boxes);
  for(i=0;i<count;i++)
  {
    s = gfc_list_get_nth(uba.boxes,i);
    gf2d_ui_box_draw(s);
  }
}

void gf2d_ui_box_draw(BoxInfo *s)
{
  Vector4D v4;
  Vector2D * v2;
  SDL_Rect rect;

  if(s->image_or_rect == 1)
  {
    v4 = vector4d(s->xOffset,s->yOffset,s->width,s->height);
    v2 = (Vector2D * )malloc(sizeof(Vector2D));
    v2->x = (float)s->resizex;
    v2->y = (float)s->resizey;
    //slog("x is %f y is %f",v2->x,v2->y);
    gf2d_sprite_draw(s->s,vector2d(s->posx,s->posy),v2,NULL,
    &v4,NULL,NULL,NULL,0);
    free(v2);
  }
  else
  {
    rect.x = s->posx;
    rect.y = s->posy;
    rect.w = s->width;
    rect.h = s->height;
    gf2d_draw_fill_rect(rect,s->color);
  }
}

void gf2d_ui_box_close()
{
  int count,i;
  count = gfc_list_get_count(uba.boxes);
  for(i=0;i<count;i++)
  {
    gf2d_ui_box_clear_box_info(gfc_list_get_nth(uba.boxes,i));
  }
  gfc_list_delete(uba.boxes);
}

void gf2d_ui_box_clear_box_info(BoxInfo * si)
{
  free(si);
}
