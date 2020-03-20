#ifndef __GF2D_CONTROLS_STATE_H_
#define __GF2D_CONTROLS_STATE_H_
#include <SDL.h>
#include "gfc_vector.h"
#include "gf2d_entity.h"
void gf2d_controls_manager_init();
void gf2d_controls_update();
Vector2D gf2d_mouse_pos();
Bool gf2d_left_mouse_down();
Bool gf2d_right_mouse_down();
Bool gf2d_key_pressed(SDL_Scancode scancode);
void gf2d_update_mouse_position(Entity *e);
cpShapeFilter gf2d_mouseFilter();
Bool gf2d_right_released();
Bool gf2d_left_released();
void gf2d_set_left_released(Bool b);
void gf2d_set_right_released(Bool b);
void gf2d_set_open_file(Bool b);
SDL_Event gf2d_get_event();
void gf2d_update_mouse_state();
void gf2d_set_mouse_colliding(Bool m);

#endif
