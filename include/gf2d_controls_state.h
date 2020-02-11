#ifndef __GF2D_CONTROLS_STATE_H_
#define __GF2D_CONTROLS_STATE_H_
#include <SDL.h>
#include "gfc_vector.h"

void gf2d_controls_manager_init();
void gf2d_controls_update();
Vector2D gf2d_mouse_pos();
Bool gf2d_left_mouse_down();
Bool gf2d_right_mouse_down();
Bool gf2d_key_pressed(SDL_Scancode scancode);

#endif
