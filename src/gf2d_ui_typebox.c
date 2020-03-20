#include "gf2d_ui_typebox.h"


typedef struct
{
  List * typeboxes;
}TypeBoxAssets;

static TypeBox * selected;

static TypeBoxAssets tba = {0};
static int typecount = 0;
static int maxButtCount = -1;
static int minButtCount = -1;
static int updatetime = 0;
static int firstUpdate = 0;
void gf2d_ui_typebox_init()
{
  tba.typeboxes = gfc_list_new();
  atexit(gf2d_ui_typebox_close);
}

void gf2d_ui_typebox_load(SJson * typeboxes)
{
  int i,count,posx,posy,width,height;
  char * font_filename;
  SJson * value;
  TypeBox * tb;
  count = sj_array_get_count(typeboxes);
  for(i = 0;i<count;i++)
  {
    value = sj_array_get_nth(typeboxes,i);
    font_filename = gf2d_ui_helper_functions_get_object_value_as_string(value,"font-filename");
    posx = atoi(gf2d_ui_helper_functions_get_object_value_as_string(value,"posx"));
    posy = atoi(gf2d_ui_helper_functions_get_object_value_as_string(value,"posy"));
    width = atoi(gf2d_ui_helper_functions_get_object_value_as_string(value,"width"));
    height = atoi(gf2d_ui_helper_functions_get_object_value_as_string(value,"height"));
    tb = gf2d_ui_typebox_init_typebox(height,width,posx,posy,font_filename);
    if(gf2d_ui_helper_functions_get_object_value_as_string(value,"char_limit") != NULL)
    {
      tb->char_limit = atoi(gf2d_ui_helper_functions_get_object_value_as_string(value,"char_limit"));
    }
    else
    {
      tb->char_limit = -1;
    }
    tb->buttRefCount = tb->bi->refcount;
    if(maxButtCount == -1)
    {
      minButtCount = tb->bi->refcount;
    }
    slog("char_limit is %d",tb->char_limit);
    maxButtCount = tb->bi->refcount;
    gf2d_ui_button_set_func_name(value,tb->bi);
    slog("the new typeboxes text is %s",tb->text);
  }
}

TypeBox * gf2d_ui_typebox_new()
{
  TypeBox * temp = (TypeBox*)malloc(sizeof(TypeBox));
  temp->refcount = typecount;
  typecount = typecount + 1;
  tba.typeboxes = gfc_list_append(tba.typeboxes,temp);
  return temp;
}

TypeBox * gf2d_ui_typebox_init_typebox(int height,int width,int posx,int posy, char * font_filename)
{
  TypeBox * temp = gf2d_ui_typebox_new();
  BoxInfo * box;
  Vector4D color = {255,255,255,255};
  char * str;
  str = (char*)malloc(sizeof(char)+1);
  strcpy(str,"");
  temp->text = str;
  temp->selected = 0;
  temp->font_filename = font_filename;
  box = gf2d_ui_box_init_box(NULL,0,0,width,height,1,1,posx,posy,0,color,0);
  temp->bi = gf2d_ui_button_new();
  temp->bi->s1 = NULL;
  temp->bi->s2 = NULL;
  temp->bi->boxInfo = box;
  temp->text_height = height*.8;
  gf2d_ui_button_setup_collision_body(temp->bi);
  temp->sprite = gf2d_ui_textbox_load_sprite(temp->text,font_filename,height*.8);
  return temp;
}

void gf2d_ui_typebox_update()
{

  SDL_Event e = gf2d_get_event();
  gf2d_set_mouse_colliding(true);
  gf2d_controls_update();
  if(selected && gf2d_left_released())
  {
    selected->selected = 0;
    selected = NULL;
    SDL_StopTextInput();
    firstUpdate = 0;
    //gf2d_set_left_released(false);

  }
  else if( selected && e.type == SDL_KEYDOWN && e.key.keysym.scancode == SDL_SCANCODE_BACKSPACE && strlen(selected->text)>0)
  {
    //slog("trying to delete text");
    if(SDL_GetTicks() - updatetime>150)
    {
      firstUpdate = 1;
      updatetime = SDL_GetTicks();
      gf2d_ui_typebox_delete_text(selected);
    }
  }
  else if(selected && e.type == SDL_TEXTINPUT)
  {

    if(selected->char_limit == -1 || selected->char_limit > strlen(selected->text))
    {
      if(firstUpdate==0)
      {
        firstUpdate = 1;
        updatetime = SDL_GetTicks();
        //slog("adding second text %s",e.text.text);
        gf2d_ui_typebox_update_text(selected,e.text.text);
      }
      else if(firstUpdate == 1)
      {
        if(SDL_GetTicks() - updatetime>150)
        {
          //firstUpdate = 2;
          updatetime = SDL_GetTicks();
          //slog("adding second text %s",e.text.text);
          gf2d_ui_typebox_update_text(selected,e.text.text);
        }
      }
    }
  }

  gf2d_set_mouse_colliding(false);
}

void gf2d_ui_typebox_update_text(TypeBox * tb,char * text)
{
  int height,strlength;
  strlength = strlen(text) + strlen(tb->text);
  tb->text = (char *)realloc(tb->text,strlength+1);
  strcat(tb->text,text);
  height = tb->text_height;
  //slog("new text is %s",tb->text);
  tb->sprite = gf2d_ui_textbox_load_sprite(tb->text,tb->font_filename,height);
}

void gf2d_ui_typebox_delete_text(TypeBox *tb)
{
  int height,strlength;
  strlength = strlen(tb->text);
  //slog("strlength is %d",strlength);
  if(strlength == 0)
    return;

  tb->text[strlength-1] = '\0';
  tb->text = (char *)realloc(tb->text,strlength);
  //strcat(tb->text,text);
  height = tb->text_height;
  //slog("new text is %s",tb->text);
  tb->sprite = gf2d_ui_textbox_load_sprite(tb->text,tb->font_filename,height);
}

void gf2d_ui_typebox_clicked(void * self)
{
  ButtonInfo * butt;
  butt = (ButtonInfo *)self;
  int pos = butt->refcount-minButtCount ;
  //slog("pos is %d",pos);
  selected = gfc_list_get_nth(tba.typeboxes,pos);
  //slog("selfs text is %s",((TypeBox *)(self))->text);
  //slog("selecteds text is %s",selected->text);
  //self->selected = 1;
  SDL_StartTextInput();
}

void gf2d_ui_typebox_draw(TypeBox *tb)
{
  int y,x;
  y = (tb->bi->boxInfo->posy);
  x = (tb->bi->boxInfo->posx)+5;
  Vector2D pos = vector2d(x,y);
  gf2d_sprite_draw_image(tb->sprite,pos);//(tb->sprite,pos,NULL,NULL,NULL,NULL,NULL,NULL,0);
}

void gf2d_ui_typebox_draw_all()
{
  int i,count;
  TypeBox * temp;
  count = gfc_list_get_count(tba.typeboxes);
  for(i=0;i<count;i++)
  {
    temp = gfc_list_get_nth(tba.typeboxes,i);
    gf2d_ui_typebox_draw(temp);
  }
}

TypeBox * gf2d_ui_typebox_get_typebox(int ind)
{
  TypeBox * temp;
  temp = gfc_list_get_nth(tba.typeboxes,ind);
  return temp;
}

void gf2d_ui_typebox_close()
{
  int i,count;
  TypeBox *temp;
  count = gfc_list_get_count(tba.typeboxes);
  for(i = 0;i<count;i++)
  {
    temp = gfc_list_get_nth(tba.typeboxes,i);
    free(temp->text);
    free(temp);
  }
  gfc_list_delete(tba.typeboxes);
}
