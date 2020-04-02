#include "gf2d_ui_textbox.h"

typedef struct
{
  List * textboxes;
}TxtBx;

static TxtBx tb = {0};

void gf2d_ui_textbox_init()
{
  tb.textboxes = gfc_list_new();
  TTF_Init();
  atexit(gf2d_ui_textbox_close);
}

void gf2d_ui_textbox_load(SJson * textboxes)
{
  int i,count,height,width,posx,posy;
  char *file_location,*text;
  SJson *value;
  Vector4D color;

  count = sj_array_get_count(textboxes);
  //slog("count is %d",count);
  for(i=0;i<count;i++)
  {
    value = sj_array_get_nth(textboxes,i);
    height = atoi(gf2d_ui_helper_functions_get_object_value_as_string(value,"height"));
    width = atoi(gf2d_ui_helper_functions_get_object_value_as_string(value,"width"));
    file_location = gf2d_ui_helper_functions_get_object_value_as_string(value,"font-filename");
    text = gf2d_ui_helper_functions_get_object_value_as_string(value,"text");

    if(gf2d_ui_helper_functions_get_object_value_as_string(value,"color.x") !=NULL)
    {
      color.x = atoi(gf2d_ui_helper_functions_get_object_value_as_string(value,"color.x"));
      color.y = atoi(gf2d_ui_helper_functions_get_object_value_as_string(value,"color.y"));
      color.z = atoi(gf2d_ui_helper_functions_get_object_value_as_string(value,"color.z"));
      color.w = atoi(gf2d_ui_helper_functions_get_object_value_as_string(value,"color.w"));
    }
    else
    {
      color.x = color.y = color.z = 0;
      color.w = 255;
    }


    posx = atoi(gf2d_ui_helper_functions_get_object_value_as_string(value,"posx"));
    posy = atoi(gf2d_ui_helper_functions_get_object_value_as_string(value,"posy"));
    gf2d_ui_textbox_init_textbox(file_location,height,width,text,posx,posy,color);
  }
}

TextBox * gf2d_ui_textbox_new()
{
  TextBox * text = (TextBox*)malloc(sizeof(TextBox));
  text->text = NULL;
  text->sprite = NULL;
  text->height = 0;
  text->width = 0;
  tb.textboxes = gfc_list_append(tb.textboxes,text);
  return text;
}

TextBox *gf2d_ui_textbox_init_textbox(char * file_location,int height,int width,char * text,int posx,int posy,Vector4D color)
{
  //Sprite * sprite;
  //BoxInfo * box;
  //SJson *tile_map_info,*sprite_info;
  //char * json_location;
  //int sprites_per_line;

  TextBox * txt = gf2d_ui_textbox_new();
  txt->text = text;
  txt->height = height;
  txt->width = width;
  txt->pos.x = posx;
  txt->pos.y = posy;
  txt->color = color;
  txt->sprite = gf2d_ui_textbox_load_sprite(text,file_location,height,color);
  return txt;
}

Sprite * gf2d_ui_textbox_load_sprite(char * text,char * file_location,int h,Vector4D c)
{
  Sprite *s;
  TTF_Font * font;
  SDL_Surface * textSurface;
  SDL_Texture * texture;
  SDL_Color color = {c.x,c.y,c.z,c.w};
  int width,height;
  font = TTF_OpenFont(file_location,h);
  if(text==NULL || strcmp(text,"")==0 )
    textSurface = TTF_RenderText_Solid(font," ",color);
  else
    textSurface = TTF_RenderText_Solid(font,text,color);
  width = textSurface->w;
  height = textSurface->h;

  textSurface = gf2d_graphics_screen_convert(&textSurface);

  texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(),textSurface);
  s = gf2d_sprite_new();
  s->texture = texture;
  SDL_SetTextureBlendMode(s->texture,SDL_BLENDMODE_BLEND);
  SDL_UpdateTexture(s->texture,
                  NULL,
                  textSurface->pixels,
                  textSurface->pitch);


  s->frame_w = width;
  s->frame_h = height;
  //s->filepath = "";
  s->ref_count = 1;
  s->frames_per_line = 1;
  SDL_FreeSurface(textSurface);
  return s;
}
void gf2d_ui_textbox_draw_all()
{
  int count,i;
  TextBox * box;
  count = gfc_list_get_count(tb.textboxes);
  //slog("drawing textboxes");
  for(i=0;i<count;i++)
  {
    box = gfc_list_get_nth(tb.textboxes,i);
    gf2d_ui_textbox_draw(box);
  }
}

void gf2d_ui_textbox_draw(TextBox * txt)
{
  //int i,count;
  //char c;
  //Vector2D temp;
  gf2d_sprite_draw(txt->sprite,txt->pos,NULL,NULL,NULL,NULL,NULL,NULL,0);


}

void gf2d_ui_textbox_clear_list()
{
  int count,i;
  TextBox * text;
  count = gfc_list_get_count(tb.textboxes);
  for(i=0;i<count;i++)
  {
    text = gfc_list_get_nth(tb.textboxes,i);
    free(text->text);
    free(text);
  }
  free(tb.textboxes);
  tb.textboxes = gfc_list_new();
}

void gf2d_ui_textbox_close()
{
  int count,i;
  TextBox * text;
  count = gfc_list_get_count(tb.textboxes);
  for(i=0;i<count;i++)
  {
    text = gfc_list_get_nth(tb.textboxes,i);
    free(text->text);
    free(text);
  }
  TTF_Quit();
  gfc_list_delete(tb.textboxes);
}
