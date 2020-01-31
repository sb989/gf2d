#include "gf2d_point.h"


Point * gf2d_point_new(int y_value)
{
  Point *temp;
  temp = gf2d_point_new_size(16);
  temp->y_value = y_value;
  return temp;
}

Point * gf2d_point_new_size(int size)
{
  Point * p = (Point *) malloc(sizeof(Point));
  memset(p,0,sizeof(Point));
  p->p_size = size;
  p->p = (SDL_Point *) malloc(sizeof(SDL_Point)*size);
  memset(p->p,0,sizeof(SDL_Point)*size);
  return p;
}

void gf2d_point_insert(Point * p,SDL_Point point)
{
  int i;
  //SDL_Point *temp;
  if(p->p_count == p->p_size -1)
  {
    p = gf2d_point_expand(p);
  }
  for(i=0;i<p->p_count;i++)
  {
    //temp = p->p[i-1];
    if(point.x < p->p[0].x)
    {
      gf2d_point_shift(p);
      p->p[0] = point;
      break;
    }
  }
}

void gf2d_point_delete(Point *p)
{
  if(!p)
  {
    return;
  }
  if(p->p)
  {
    free(p->p);
  }
  free(p);
}

Point * gf2d_point_expand(Point * p)
{
  Point * new = gf2d_point_new_size(p->p_size*2);
  memcpy(new->p,p->p,sizeof(SDL_Point)*p->p_size);
  new->max_x = p->max_x;
  new->min_x = p->min_x;
  new->y_value = p->y_value;
  new->p_count = p->p_count;
  gf2d_point_delete(p);
  return new;
}
void gf2d_point_shift(Point * p)
{
  int i;
  for(i = p->p_count-1;i>=0;i--)
  {
    p->p[i+1] = p->p[i];
  }
}
