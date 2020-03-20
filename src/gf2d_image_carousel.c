#include "gf2d_image_carousel.h"

static List *carousels = NULL;
static int car_count;

void gf2d_image_carousel_init()
{
  carousels = gfc_list_new();
  car_count = 0;
  atexit(gf2d_image_carousel_free);
}

Carousel * gf2d_image_carousel_init_carousel(int width,int height,int spacing,int posx,int posy)
{
  ButtonInfo *left,*right;
  Carousel * temp = gf2d_image_carousel_new_carousel();
  Sprite * shift;
  if(width < 48+height)
  {
    slog("width must be at least 48 + the inputted height(%d),returing null.",height);
  }
  temp->width = width;
  temp-> height = height;
  temp->posx = posx;
  temp->posy = posy;
  temp->spacing = spacing;
  temp->selected = NULL;
  shift = gf2d_sprite_load_all("images/dotRed.png",24,24,1);
  left = gf2d_image_carousel_button_init(shift,0,posx,posy,&gf2d_image_carousel_shift_right,NULL,1,1);
  right = gf2d_image_carousel_button_init(shift,0,posx+width-24,posy,&gf2d_image_carousel_shift_left,NULL,1,1);
  temp->left_width = 24;
  temp->right_width = 24;
  left->boxInfo->render = 0;
  right->boxInfo->render =0;
  left->holdernum = car_count;
  right->holdernum = car_count;
  temp->left = left;
  temp->right = right;
  temp->new_posx = temp->left_width+posx;
  temp->left_button_ref_count = left->refcount;
  temp->right_button_ref_count = right->refcount;
  temp->carouselElements = gfc_list_new();
  temp->car_num = car_count;
  temp->moving = 0;
  car_count = car_count+1;
  slog("created a carousel");
  return temp;
}

void gf2d_image_carousel_add_carousel_element(Carousel * c,Sprite * sprite,int sprite_num,void(*onHold)(void*),void(*onRelease)(void*))
{
  int posx = c->new_posx;
  //slog("posx is %d",posx);
  float resizex = .7*((float)c->height)/((float)sprite->frame_w);
  float resizey = .7*((float)c->height)/((float)sprite->frame_h);
  CarouselElement * ce = gf2d_image_carousel_init_carousel_element(sprite,sprite_num,posx,c->posy,onHold,onRelease,resizex,resizey);
  ce->butt->holdernum = c->car_num;
  ce->posx = posx;
  //ce->refcount = c->num_elements;
  c->new_posx = c->new_posx+c->height;
  //slog("new posx is %d",c->new_posx);
  gf2d_image_carousel_update_element_visibility(ce,c);
  c->carouselElements = gfc_list_append(c->carouselElements,ce);
}

CarouselElement * gf2d_image_carousel_init_carousel_element(Sprite * sprite,int sprite_num,int posx,int posy,void(*onHold)(void*),void (*onRelease)(void*),float resizex,float resizey)
{
  CarouselElement * ce = gf2d_image_carousel_new_carousel_element();
  ce->butt = gf2d_image_carousel_button_init(sprite,sprite_num,posx,posy,onHold,onRelease,resizex,resizey);
  return ce;
}

ButtonInfo * gf2d_image_carousel_button_init(Sprite *sprite,int sprite_num,int posx,int posy,void(*onHold)(void*),void(*onRelease)(void*),float resizex,float resizey)
{
  //Sprite * sprite;
  BoxInfo *box;
  ButtonInfo *button = gf2d_ui_button_new();
  //sprite = gf2d_sprite_load_all(file_location,w,h,spl);
  box = gf2d_ui_box_init_box(sprite,0,0,sprite->frame_w,sprite->frame_h,resizex,resizey,posx,posy,2,vector4d(0,0,0,0),sprite_num);
  button->boxInfo = box;
  //button->func_name = ;
  button->onHold = onHold;
  button->onRelease = onRelease;
  gf2d_ui_button_setup_collision_body(button);
  return button;
}

void gf2d_image_carousel_set_selected(void *button)
{
  ButtonInfo *butt = (ButtonInfo*)button;
  Carousel *c = gfc_list_get_nth(carousels,butt->holdernum);
  //slog("holdernum is %d",butt->holdernum);
  //slog("time is %d",SDL_GetTicks());
  c->selected = butt;
}

Carousel * gf2d_image_carousel_new_carousel()
{
  Carousel * temp = (Carousel*) malloc(sizeof(Carousel));
  carousels = gfc_list_append(carousels,temp);
  return temp;
}

CarouselElement * gf2d_image_carousel_new_carousel_element()
{
  CarouselElement * temp = (CarouselElement*) malloc(sizeof(CarouselElement));
  return temp;
}

void gf2d_image_carousel_shift_left(void *right)
{
  Carousel *c;
  CarouselElement *ce;
  int i,count;
  cpVect vel;
  vel.x = -50;
  vel.y = 0;
  c = gfc_list_get_nth(carousels,((ButtonInfo*)right)->holdernum);//c = gf2d_image_carousel_find_right_button_carousel((ButtonInfo*)right);
  if(!c)
    return;
  count = gfc_list_get_count(c->carouselElements);

  for(i = 0; i<count;i++)
  {
    ce = gfc_list_get_nth(c->carouselElements,i);
    ce->posx = ce->posx-10;
    ce->butt->boxInfo->posx = ce->posx;
  //  cpBodySetVelocity(ce->butt->body,vel);
    gf2d_image_carousel_update_element_visibility(ce,c);
    if(ce->render == 1)
      gf2d_ui_button_update_collision_body(ce->butt);
  }
  c->new_posx = c->new_posx-10;
  c->moving = 1;
}

void gf2d_image_carousel_shift_right(void *left)
{
  Carousel *c;
  CarouselElement *ce;
  int i,count;
  //cpVect vel;
  //vel.x = 50;
  //vel.y = 0;
  c = gfc_list_get_nth(carousels,((ButtonInfo*)left)->holdernum);//gf2d_image_carousel_find_left_button_carousel((ButtonInfo*)left);
  if(!c)
    return;
  count = gfc_list_get_count(c->carouselElements);
  for(i = 0; i<count;i++)
  {
    ce = gfc_list_get_nth(c->carouselElements,i);
    ce->posx = ce->posx+10;
    ce->butt->boxInfo->posx = ce->posx;
    //cpBodySetVelocity(ce->butt->body,vel);
    gf2d_image_carousel_update_element_visibility(ce,c);
    if(ce->render ==1)
      gf2d_ui_button_update_collision_body(ce->butt);
  }
  c->new_posx = c->new_posx+10;
  c->moving = 1;
}

void gf2d_image_carousel_stop_moving(void *butt)
{
  Carousel *c;
  int i,count;
  cpVect vel;
  CarouselElement *ce;
  vel.x = 0;
  vel.y = 0;
  ButtonInfo * b = (ButtonInfo *) butt;
  c = gfc_list_get_nth(carousels,b->holdernum);
  count = gfc_list_get_count(c->carouselElements);
  for(i = 0; i<count;i++)
  {
    ce = gfc_list_get_nth(c->carouselElements,i);
    cpBodySetVelocity(ce->butt->body,vel);
  }
  c->moving = 0;

}

void gf2d_image_carousel_update_element_visibility_all(Carousel *c)
{
  int i,count;
  CarouselElement *ce;
  count = gfc_list_get_count(c->carouselElements);
  for(i = 0; i<count;i++)
  {
    ce = gfc_list_get_nth(c->carouselElements,i);
    gf2d_image_carousel_update_element_visibility(ce,c);
  }
}

void gf2d_image_carousel_update_element_visibility(CarouselElement *ce,Carousel *c)
{
  int posx;
  posx = ce->butt->boxInfo->posx;//posx;
  if( (posx+ c->height > c->posx + c->left_width) && (posx < (c->posx)+(c->width)-c->right_width) )
    ce->render = 1;
  else
    ce->render = 0;
  ce->butt->boxInfo->render = ce->render;

}

Carousel * gf2d_image_carousel_find_left_button_carousel(ButtonInfo * left)
{
  //Carousel * c;
  int count;
  count = gfc_list_get_count(carousels);
  if(count < left->holdernum)
    return NULL;
  else
  {
    return gfc_list_get_nth(carousels,left->holdernum);
  }
}

Carousel *gf2d_image_carousel_find_right_button_carousel(ButtonInfo * right)
{
  //Carousel * c;
  int count;
  count = gfc_list_get_count(carousels);
  if(count < right->holdernum)
    return NULL;
  else
  {
    return gfc_list_get_nth(carousels,right->holdernum);
  }
}

void gf2d_image_carousel_draw_all_lr()
{
  int i,count;
  Carousel *c;
  count = gfc_list_get_count(carousels);
  for(i=0;i<count;i++)
  {
    c = gfc_list_get_nth(carousels,i);
    gf2d_image_carousel_draw_lr(c);
  }
}

void gf2d_image_carousel_update_element_position_all(Carousel *c)
{
  int i,count,diff;
  CarouselElement * ce;
  count = gfc_list_get_count(c->carouselElements);
  for(i=0;i<count;i++)
  {
      ce = gfc_list_get_nth(c->carouselElements,i);
      diff = ce->butt->boxInfo->posx;
      gf2d_ui_button_update_graphics_position(ce->butt);
      diff = diff - ce->butt->boxInfo->posx;
  }
  c->new_posx = c->new_posx - diff;
}


void gf2d_image_carousel_draw_lr(Carousel *c)
{
  Vector2D * v2;

  BoxInfo * s = c->left->boxInfo;
  v2 = (Vector2D * )malloc(sizeof(Vector2D));
  v2->x = (float)s->resizex;
  v2->y = (float)s->resizey;
  gf2d_sprite_draw(s->s,vector2d(s->posx,s->posy),v2,NULL,
  NULL,NULL,NULL,NULL,s->sprite_num);

  s = c->right->boxInfo;
  v2->x = (float)s->resizex;
  v2->y = (float)s->resizey;
  gf2d_sprite_draw(s->s,vector2d(s->posx,s->posy),v2,NULL,
  NULL,NULL,NULL,NULL,s->sprite_num);

  free(v2);
}

void gf2d_image_carousel_free()
{
  int i,count;
  Carousel * temp;
  count = gfc_list_get_count(carousels);
  for(i=0;i<count;i++)
  {
    temp = gfc_list_get_nth(carousels,i);
    free(temp->carouselElements);
    free(temp);
  }
  free(carousels);
}
