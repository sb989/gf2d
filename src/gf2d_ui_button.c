#include "gf2d_ui_button.h"

typedef struct
{
  List *buttons;
}UIButtonAssets;

static UIButtonAssets uba = {0};
static int buttCount = 0;

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
    if(strcmp(image_or_rect,"image_uneven")==0)
    {
      bi = gf2d_ui_box_info_image_uneven(temp);
      tbi->boxInfo = bi;
      gf2d_ui_button_sprite_init(temp,tbi);
      gf2d_ui_button_set_func_name(temp,tbi);
      gf2d_ui_button_setup_collision_body(tbi);
    }
    else if(strcmp(image_or_rect,"image_even")==0)
    {
      bi = gf2d_ui_box_info_image_even(temp);
      tbi->boxInfo = bi;
      gf2d_ui_button_sprite_init(temp,tbi);
      gf2d_ui_button_set_func_name(temp,tbi);
      gf2d_ui_button_setup_collision_body(tbi);
    }
    else
    {
      bi = gf2d_ui_box_info_rect(temp);
      tbi->boxInfo = bi;
      tbi->s1 = NULL;
      tbi->s2 = NULL;
      gf2d_ui_button_set_func_name(temp,tbi);
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



void gf2d_ui_button_set_func_name_left(SJson * value,ButtonInfo *tbi)
{
  //gf2d_ui_button_set_func_name_temp(value,tbi,tbi->onReleaseName,tbi->onHoldName,tbi->onRelease_data,tbi->onHold_data,tbi->onRelease,tbi->onHold);
  char * onRelease,*onHold;
  char * onRelease_data,*onHold_data;
  Function * oh, * or;
  onRelease = gf2d_ui_helper_functions_get_object_value_as_string(value,"onReleaseName");
  tbi->onReleaseName = onRelease;
  or = gf2d_find_functions_find_func(onRelease);
  if(or)
    tbi->onRelease = or->func;
  else
    tbi->onRelease = NULL;
  onRelease_data = gf2d_ui_helper_functions_get_object_value_as_string(value,"onReleaseData");
  if(!onRelease_data)
    tbi->onRelease_data = NULL;
  else if(strcmp(onRelease_data,"NULL")== 0)
   tbi->onRelease_data = NULL;
  else
  {
    tbi->onRelease_data = NULL;
  }

  onHold = gf2d_ui_helper_functions_get_object_value_as_string(value,"onHoldName");
  tbi->onHoldName = onHold;
  oh = gf2d_find_functions_find_func(onHold);
  if(oh)
    tbi->onHold = oh->func;
  else
    tbi->onHold = NULL;
  onHold_data = gf2d_ui_helper_functions_get_object_value_as_string(value,"onHoldData");
  if(!onHold_data)
    tbi->onHold_data = NULL;
  else if(strcmp(onHold_data,"NULL")== 0)
   tbi->onHold_data = NULL;
  else
  {
    tbi->onHold_data = NULL;
  }

}

void gf2d_ui_button_set_func_name_right(SJson * value,ButtonInfo *tbi)
{
    char * onRelease,*onHold;
    char * onRelease_data,*onHold_data;
    Function * oh, * or;
    onRelease = gf2d_ui_helper_functions_get_object_value_as_string(value,"rightOnReleaseName");
    tbi->rightOnReleaseName = onRelease;
    or = gf2d_find_functions_find_func(onRelease);
    if(or)
      tbi->rightOnRelease = or->func;
    else
      tbi->rightOnRelease = NULL;
    onRelease_data = gf2d_ui_helper_functions_get_object_value_as_string(value,"rightOnReleaseData");
    if(!onRelease_data)
      tbi->rightOnRelease_data = NULL;
    else if(strcmp(onRelease_data,"NULL")== 0)
     tbi->rightOnRelease_data = NULL;
    else
    {
      tbi->rightOnRelease_data = NULL;
    }

    onHold = gf2d_ui_helper_functions_get_object_value_as_string(value,"rightOnHoldName");
    tbi->rightOnHoldName = onHold;
    oh = gf2d_find_functions_find_func(onHold);
    if(oh)
      tbi->rightOnHold = oh->func;
    else
      tbi->rightOnHold = NULL;
    onHold_data = gf2d_ui_helper_functions_get_object_value_as_string(value,"rightOnHoldData");
    if(!onHold_data)
      tbi->rightOnHold_data = NULL;
    else if(strcmp(onHold_data,"NULL")== 0)
     tbi->rightOnHold_data = NULL;
    else
    {
      tbi->rightOnHold_data = NULL;
    }
}


void gf2d_ui_button_set_func_name(SJson * value,ButtonInfo *tbi)
{
  gf2d_ui_button_set_func_name_left(value,tbi);
  gf2d_ui_button_set_func_name_right(value,tbi);
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
  free(xml_tagname);
  free(json_location);

}

void gf2d_ui_button_setup_collision_body(ButtonInfo *b)
{
  cpFloat length,width,radius;
  cpVect pos;
  int x,y;
  float resizex,resizey;
  cpShapeFilter filter = gf2d_ui_button_filter();
  resizex = b->boxInfo->resizex;
  resizey = b->boxInfo->resizey;
  //slog("resizex is %f\tresizey is %f",resizex,resizey);
  length = b->boxInfo->height * resizey;
  width = b->boxInfo->width * resizex;
  //slog("length is %f width is %f",length,width);
  radius = 0;
  b->shape = gf2d_physics_add_square_body(length,width,radius,0);
  b->body = cpShapeGetBody(b->shape);
  cpShapeSetCollisionType(b->shape,BUTTON);
  cpBodySetUserData(b->body,b);
  x = (float)b->boxInfo->posx;
  y = (float)b->boxInfo->posy;

  pos = cpv(x,y);

  cpBodySetPosition(b->body,pos);
  cpSpaceReindexShapesForBody(cpBodyGetSpace(b->body),b->body);
  cpShapeSetFilter(b->shape,filter);
}

void gf2d_ui_button_update_collision_body(ButtonInfo *b)
{
  cpVect pos;
  pos = cpv(b->boxInfo->posx,b->boxInfo->posy);
  cpBodySetPosition(b->body,pos);
  //cpSpaceReindexShapesForBody(cpBodyGetSpace(b->body),b->body);

}

void gf2d_ui_button_update_graphics_position_all()
{
    int i,count;
    ButtonInfo * butt;
    count = gfc_list_get_count(uba.buttons);
    for(i=0;i<count;i++)
    {
      butt = gfc_list_get_nth(uba.buttons,i);
      gf2d_ui_button_update_graphics_position(butt);
    }
}

void gf2d_ui_button_update_graphics_position(ButtonInfo *b)
{
  cpVect pos = cpBodyGetPosition(b->body);
  b->boxInfo->posx = pos.x;
  b->boxInfo->posy = pos.y;
}

cpShapeFilter gf2d_ui_button_filter()
{
  cpGroup group;
  cpBitmask mask;
  cpBitmask cat;
  cpShapeFilter filter;
  group = 0;
  mask = MOUSE;
  cat = BUTTON;
  filter = cpShapeFilterNew(group,cat,mask);
  return filter;
}



ButtonInfo * gf2d_ui_button_new()
{

  ButtonInfo * temp = (ButtonInfo*) malloc(sizeof(ButtonInfo));

  temp->onHoldName = NULL;
  temp->onReleaseName = NULL;
  temp->rightOnHoldName = NULL;
  temp->rightOnReleaseName = NULL;

  temp->onRelease = NULL;
  temp->onHold = NULL;
  temp->rightOnHold = NULL;
  temp->rightOnRelease= NULL;

  temp->boxInfo = NULL;
  temp->s1 = NULL;
  temp->s2 = NULL;

  temp->shape = NULL;
  temp->body = NULL;
  temp->refcount = buttCount;
  buttCount = buttCount+1;
  uba.buttons = gfc_list_append(uba.buttons,temp);
  return temp;
}

void gf2d_ui_button_clear_list()
{
  int count,i;
  ButtonInfo * temp;
  count = gfc_list_get_count(uba.buttons);
  for(i = 0;i<count;i++)
  {
    temp = gfc_list_get_nth(uba.buttons,i);
    if(temp->onHoldName)
      free(temp->onHoldName);
    if(temp->onReleaseName)
      free(temp->onReleaseName);
    if(temp->rightOnHoldName)
      free(temp->rightOnHoldName);
    if(temp->rightOnReleaseName)
      free(temp->rightOnReleaseName);
    if(temp->shape && temp->body)
      gf2d_ui_button_free_body(temp);
    free(temp);
  }
  free(uba.buttons);
  uba.buttons = gfc_list_new();
}

void gf2d_ui_button_close()
{
  int count,i;
  ButtonInfo * temp;
  count = gfc_list_get_count(uba.buttons);
  for(i = 0;i<count;i++)
  {
    temp = gfc_list_get_nth(uba.buttons,i);
    if(temp->onHoldName)
      free(temp->onHoldName);
    if(temp->onReleaseName)
      free(temp->onReleaseName);
    if(temp->rightOnHoldName)
      free(temp->rightOnHoldName);
    if(temp->rightOnReleaseName)
      free(temp->rightOnReleaseName);
    if(temp->shape && temp->body)
      gf2d_ui_button_free_body(temp);
    free(temp);
  }
  gfc_list_delete(uba.buttons);
}

void gf2d_ui_button_free_body(ButtonInfo * butt)
{
  cpSpace * space = gf2d_physics_get_space();
  cpSpaceRemoveShape(space, butt->shape);
  cpSpaceRemoveBody(space, butt->body);
  cpShapeFree(butt->shape);
  cpBodyFree(butt->body);
}
