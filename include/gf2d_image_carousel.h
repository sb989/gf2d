#ifndef __GF2D_IMAGE_CAROUSEL_H_
#define __GF2D_IMAGE_CAROUSEL_H_
#include "gfc_list.h"
#include "gf2d_ui_button.h"
#include "../Chipmunk-7.0.3/include/chipmunk/chipmunk.h"

typedef struct
{
  ButtonInfo * butt;
  int posx; // x position to render at if render is selected
  int render; //0 to not draw 1 to draw
  //int refcount;
}CarouselElement;


typedef struct
{
  List *carouselElements;
  int width,height,posx,posy,image_h,image_w,new_posx,left_button_ref_count,right_button_ref_count;
  int spacing;
  int car_num;
  int left_width;
  int right_width;
  int moving;
  ButtonInfo *left,*right,*selected;
}Carousel;

void gf2d_image_carousel_init();
Carousel * gf2d_image_carousel_init_carousel(int width,int height,int spacing,int posx,int posy);
CarouselElement * gf2d_image_carousel_init_carousel_element(Sprite * sprite,int sprite_num,int posx,int posy,void(*onHold)(void*),void(*onRelease)(void*),float resizex,float resizey);
void gf2d_image_carousel_add_carousel_element(Carousel * c,Sprite * sprite,int sprite_num,void(*onHold)(void*),void(*onRelease)(void*));
void gf2d_image_carousel_shift_left(void *left);
void gf2d_image_carousel_shift_right(void *right);
Carousel * gf2d_image_carousel_find_left_button_carousel(ButtonInfo * left);
Carousel * gf2d_image_carousel_find_right_button_carousel(ButtonInfo * right);
ButtonInfo * gf2d_image_carousel_button_init(Sprite *sprite,int sprite_num,int posx,int posy,void(*onHold)(void*),void(*onRelease)(void*),float resizex,float resizey);
Carousel * gf2d_image_carousel_new_carousel();
CarouselElement * gf2d_image_carousel_new_carousel_element();
void gf2d_image_carousel_free();
void gf2d_image_carousel_draw_lr(Carousel *c);
void gf2d_image_carousel_draw_all_lr();
void gf2d_image_carousel_set_selected(void *button);
void gf2d_image_carousel_update_element_visibility(CarouselElement *ce,Carousel *c);
void gf2d_ui_button_update_graphics_position_all();
void gf2d_image_carousel_stop_moving(void *butt);
void gf2d_image_carousel_update_element_visibility_all(Carousel *c);
void gf2d_image_carousel_update_element_position_all(Carousel *c);
void gf2d_image_carousel_clear_list();
void gf2d_image_carousel_reindex(Carousel *c);

#endif
