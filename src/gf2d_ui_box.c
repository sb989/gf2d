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
  uba.boxes = gfc_list_append(uba.boxes,temp);
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
    if(strcmp(image_or_rect,"image_uneven")==0)
    {
      gf2d_ui_box_info_image_uneven(value);
    }
    else if(strcmp(image_or_rect,"image_even")==0)
    {
      gf2d_ui_box_info_image_even(value);
    }
    else
    {
      gf2d_ui_box_info_rect(value);
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

BoxInfo * gf2d_ui_box_init_box(Sprite *s,int xOffset,int yOffset,int width,int height,float resizex,float resizey,int posx,int posy,int image_or_rect,Vector4D color,int sprite_num)
{
  BoxInfo * box = gf2d_ui_box_new();

  box->s = s;
  box->xOffset = xOffset;
  box->yOffset = yOffset;
  box->width = width;
  box->height = height;
  box->resizex = resizex;
  box->resizey = resizey;
  box->posx = posx;
  box->posy = posy;
  box->image_or_rect = image_or_rect;
  box->color = color;
  box->sprite_num = sprite_num;
  box->render = 1;
  return box;

}

BoxInfo * gf2d_ui_box_info_image_uneven(SJson * value)
{
  char *file_location,*posx,*posy,*xml_tagname,*tempy,*tempx,*resize,*json_location;
  int xOff,yOff,width,height,resizex,resizey,screen_width,screen_height;
  SJson *tile_map_info,*sprite_info;
  Sprite * temp;
  BoxInfo * temp_si;

  file_location = gf2d_ui_helper_functions_get_object_value_as_string(value,"file_location");
  posx = gf2d_ui_helper_functions_get_object_value_as_string(value,"posx");
  posy = gf2d_ui_helper_functions_get_object_value_as_string(value,"posy");
  resize = gf2d_ui_helper_functions_get_object_value_as_string(value,"resize");

  xml_tagname = gf2d_ui_helper_functions_get_object_value_as_string(value,"xml_tagname");
  json_location = gf2d_ui_helper_functions_json_file_location(file_location);
  tile_map_info = sj_load(json_location);

  sprite_info = gf2d_ui_box_get_sprite_info_from_json(tile_map_info,xml_tagname);

  xOff = atoi(gf2d_ui_helper_functions_get_object_value_as_string(sprite_info,"@x"));
  yOff = atoi(gf2d_ui_helper_functions_get_object_value_as_string(sprite_info,"@y"));
  width = atoi(gf2d_ui_helper_functions_get_object_value_as_string(sprite_info,"@width"));
  height = atoi(gf2d_ui_helper_functions_get_object_value_as_string(sprite_info,"@height"));

  temp = gf2d_sprite_load_all(file_location,xOff+width,yOff+height,1);

  if(strcmp(resize,"yes")==0)
  {
    tempx = gf2d_ui_helper_functions_get_object_value_as_string(value,"resize_x");
    tempy = gf2d_ui_helper_functions_get_object_value_as_string(value,"resize_y");
    SDL_Window *win = gf2d_graphics_get_window();
    SDL_GetWindowSize(win,&screen_width,&screen_height);
    if(strcmp(tempx,"screen_width")==0)
    {
      resizex = screen_width;
    }
    else
    {
      resizex = atoi(tempx);
    }
    if(strcmp(tempy,"screen_height")==0)
    {
      resizey = screen_height;
    }
    else
    {
      resizey = atoi(tempy);
    }
  }
  else
  {
    resizex = 1;
    resizey = 1;
  }
  temp_si = gf2d_ui_box_init_box(temp,xOff,yOff,width,height,resizex,resizey,atoi(posx),atoi(posy),1,vector4d(0,0,0,0),0);
  free(json_location);
  return temp_si;
}
BoxInfo * gf2d_ui_box_info_image_even(SJson * value)
{
  char *file_location,*posx,*posy,*resize,*tempx,*tempy;
  int width,height,resizex,resizey,screen_width,screen_height,spl,sprite_num;
  Sprite * temp;
  BoxInfo * temp_si;

  file_location = gf2d_ui_helper_functions_get_object_value_as_string(value,"file_location");
  posx = gf2d_ui_helper_functions_get_object_value_as_string(value,"posx");
  posy = gf2d_ui_helper_functions_get_object_value_as_string(value,"posy");
  resize = gf2d_ui_helper_functions_get_object_value_as_string(value,"resize");
  width = atoi(gf2d_ui_helper_functions_get_object_value_as_string(value,"width"));
  height = atoi(gf2d_ui_helper_functions_get_object_value_as_string(value,"height"));
  spl =  atoi(gf2d_ui_helper_functions_get_object_value_as_string(value,"spl"));
  temp = gf2d_sprite_load_all(file_location,width,height,1);
  sprite_num = atoi(gf2d_ui_helper_functions_get_object_value_as_string(value,"sprite_num"));
  if(strcmp(resize,"yes")==0)
  {
    tempx = gf2d_ui_helper_functions_get_object_value_as_string(value,"resize_x");
    tempy = gf2d_ui_helper_functions_get_object_value_as_string(value,"resize_y");
    SDL_Window *win = gf2d_graphics_get_window();
    SDL_GetWindowSize(win,&screen_width,&screen_height);
    if(strcmp(tempx,"screen_width")==0)
    {
      resizex = screen_width;
    }
    else
    {
      resizex = atoi(tempx);
    }
    if(strcmp(tempy,"screen_height")==0)
    {
      resizey = screen_height;
    }
    else
    {
      resizey = atoi(tempy);
    }
  }
  else
  {
    resizex = 1;
    resizey = 1;
  }
  temp_si = gf2d_ui_box_init_box(temp,0,0,width,height,resizex,resizey,atoi(posx),atoi(posy),2,vector4d(0,0,0,0),sprite_num);
  return temp_si;
}



BoxInfo * gf2d_ui_box_info_rect(SJson * value)
{

  char *posx,*posy;
  int screen_width,screen_height,height,width;
  BoxInfo * temp_si;
  SDL_Window *win;
  Vector4D rcolor;

  posx = gf2d_ui_helper_functions_get_object_value_as_string(value,"posx");
  posy = gf2d_ui_helper_functions_get_object_value_as_string(value,"posy");
  win = gf2d_graphics_get_window();
  SDL_GetWindowSize(win,&screen_width,&screen_height);

  if(strcmp(gf2d_ui_helper_functions_get_object_value_as_string(value,"width"),"screen_width")==0)
  {
    width = screen_width;
  }
  else
  {
    width = atoi(gf2d_ui_helper_functions_get_object_value_as_string(value,"width"));
  }
  if(strcmp(gf2d_ui_helper_functions_get_object_value_as_string(value,"height"),"screen_height")==0)
  {
    height = screen_height;
  }
  else
  {
    height = atoi(gf2d_ui_helper_functions_get_object_value_as_string(value,"height"));
  }
  rcolor.x = atoi(gf2d_ui_helper_functions_get_object_value_as_string(value,"color.x"));
  rcolor.y = atoi(gf2d_ui_helper_functions_get_object_value_as_string(value,"color.y"));
  rcolor.z = atoi(gf2d_ui_helper_functions_get_object_value_as_string(value,"color.z"));
  rcolor.w = atoi(gf2d_ui_helper_functions_get_object_value_as_string(value,"color.w"));

  temp_si = gf2d_ui_box_init_box(NULL,0,0,width,height,1,1,atoi(posx),atoi(posy),0,rcolor,0);
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

    if(s->render == 1)
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
    &v4,NULL,NULL,NULL,s->sprite_num);
    free(v2);
  }
  else if(s->image_or_rect == 0)
  {
    rect.x = s->posx;
    rect.y = s->posy;
    rect.w = s->width;
    rect.h = s->height;
    gf2d_draw_fill_rect(rect,s->color);
  }
  else
  {
    v2 = (Vector2D * )malloc(sizeof(Vector2D));
    v2->x = (float)s->resizex;
    v2->y = (float)s->resizey;
    gf2d_sprite_draw(s->s,vector2d(s->posx,s->posy),v2,NULL,
    NULL,NULL,NULL,NULL,s->sprite_num);
    free(v2);
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
