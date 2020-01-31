#ifndef __GF2D_POINT_H_
#define __GF2D_POINT_H_
#include <SDL.h>

typedef struct
{
  int max_x;
  int min_x;
  int y_value;
  SDL_Point *p;
  int p_size;
  int p_count;
}Point;


Point * gf2d_point_new();

Point * gf2d_point_new_size(int size);

void gf2d_point_delete(Point *p);

Point * gf2d_point_expand(Point * p);

void gf2d_point_shift(Point * p);

void gf2d_point_insert(Point * p,SDL_Point point);


#endif
