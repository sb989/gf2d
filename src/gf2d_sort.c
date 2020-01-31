#include "gf2d_sort.h"


void gf2d_insertion_sort_point(SDL_Point * data,int n, int *x_max,int * x_min)
{
  int i,j,y_val,x_val;
  SDL_Point key;

  y_val = data[0].y;
  x_val = data[0].x;

  if(x_val < x_min[y_val])
  {
    x_min[y_val] = x_val;
  }

  if(x_val > x_max[y_val])
  {
    x_max[y_val] = x_val;
  }

  for(i=1;i<n;i++)
  {

    y_val = data[i].y;
    x_val = data[i].x;

    if(x_val < x_min[y_val])
    {
      x_min[y_val] = x_val;
    }
    if(x_val > x_max[y_val])
    {
      x_max[y_val] = x_val;
    }

    key = data[i];
    j = i-1;
    while(j>= 0 &&  (data[j].y >= key.y))
    {
      if(data[j].y == key.y && data[j].x < key.x) break;
      data[j+1]= data[j];
      j = j-1;
    }
    data[j+1] =  key;
  }
}
