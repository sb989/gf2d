#include "gf2d_draw.h"
#include "gf2d_graphics.h"
#include "simple_logger.h"
#include "gf2d_sort.h"
void gf2d_draw_lines(Vector2D *p1,Vector2D *p2, Uint32 lines,Vector4D color)
{
    int i;
    SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(),
                           color.x,
                           color.y,
                           color.z,
                           color.w);
    for (i = 0; i < lines;i++)
    {
        SDL_RenderDrawLine(gf2d_graphics_get_renderer(),
                           p1[i].x,
                           p1[i].y,
                           p2[i].x,
                           p2[i].y);
    }
    SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(),
                           255,
                           255,
                           255,
                           255);
}

void gf2d_draw_line(Vector2D p1,Vector2D p2, Vector4D color)
{
    SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(),
                           color.x,
                           color.y,
                           color.z,
                           color.w);
    SDL_RenderDrawLine(gf2d_graphics_get_renderer(),
                       p1.x,
                       p1.y,
                       p2.x,
                       p2.y);
    SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(),
                           255,
                           255,
                           255,
                           255);
}

void gf2d_draw_rect(SDL_Rect rect,Vector4D color)
{
    SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(),
                           color.x,
                           color.y,
                           color.z,
                           color.w);
    SDL_RenderDrawRect(gf2d_graphics_get_renderer(),(const struct SDL_Rect *)&rect);
    SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(),
                           255,
                           255,
                           255,
                           255);
}

void gf2d_draw_fill_rect(SDL_Rect rect,Vector4D color)
{
  SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(),
                          color.x,
                          color.y,
                          color.z,
                          color.w);
  SDL_RenderFillRect(gf2d_graphics_get_renderer(),(const struct SDL_Rect *)&rect);
  SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(),
                         255,
                         255,
                         255,
                         255);
}

void gf2d_draw_rects(SDL_Rect *rects,Uint32 count,Vector4D color)
{
    SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(),
                           color.x,
                           color.y,
                           color.z,
                           color.w);
    SDL_RenderDrawRects(gf2d_graphics_get_renderer(),rects,count);
}

void gf2d_draw_pixel(Vector2D pixel,Vector4D color)
{
    SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(),
                           color.x,
                           color.y,
                           color.z,
                           color.w);
    SDL_RenderDrawPoint(gf2d_graphics_get_renderer(),
                        pixel.x,
                        pixel.y);

    SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(),
                           255,
                           255,
                           255,
                           255);
}

void gf2d_draw_pixel_list(SDL_Point * pixels,Uint32 count,Vector4D color)
{
    SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(),
                           color.x,
                           color.y,
                           color.z,
                           color.w);
    SDL_RenderDrawPoints(gf2d_graphics_get_renderer(),
                        pixels,
                        count);
}

/*
 * credit circle drawing algorith:
 * http://groups.csail.mit.edu/graphics/classes/6.837/F98/Lecture6/circle.html
 */

static int gf2d_draw_circle_points(SDL_Point *p,Vector2D center, Vector2D point)
{
  if (point.x == 0)
  {
    vector2d_set(p[0],center.x, center.y + point.y);
    vector2d_set(p[1],center.x, center.y - point.y);
    vector2d_set(p[2],center.x + point.y, center.y);
    vector2d_set(p[3],center.x - point.y, center.y);
    return 4;
  }
  if (point.x == point.y)
  {
    vector2d_set(p[0],center.x + point.x, center.y + point.y);
    vector2d_set(p[1],center.x - point.x, center.y + point.y);
    vector2d_set(p[2],center.x + point.x, center.y - point.y);
    vector2d_set(p[3],center.x - point.x, center.y - point.y);
    return 4;
  }
  if (point.x < point.y)
  {
    vector2d_set(p[0],center.x + point.x, center.y + point.y);
    vector2d_set(p[1],center.x - point.x, center.y + point.y);
    vector2d_set(p[2],center.x + point.x, center.y - point.y);
    vector2d_set(p[3],center.x - point.x, center.y - point.y);
    vector2d_set(p[4],center.x + point.y, center.y + point.x);
    vector2d_set(p[5],center.x - point.y, center.y + point.x);
    vector2d_set(p[6],center.x + point.y, center.y - point.x);
    vector2d_set(p[7],center.x - point.y, center.y - point.x);
    return 8;
  }
  return 0;
}


void gf2d_draw_circle(Vector2D center, int radius, Vector4D color)
{
    SDL_Point *pointArray;
    int i = 0;
    Vector2D point = {0,0};
    int p = (5 - radius*4)/4;
    point.y = radius;
    pointArray = (SDL_Point*)malloc(sizeof(SDL_Point)*radius*8);

    if (!pointArray)
    {
        slog("gf2d_draw_circle: failed to allocate points for circle drawing");
        return;
    }
    i = gf2d_draw_circle_points(&pointArray[i],center, point);
    while (point.x < point.y)
    {
        point.x++;
        if (p < 0)
        {
            p += 2*point.x+1;
        }
        else
        {
            point.y--;
            p += 2*(point.x-point.y)+1;
        }
        i += gf2d_draw_circle_points(&pointArray[i],center, point);
        if (i + 8 >= radius*8)
        {
            slog("too many points for circle");
            break;
        }
    }
    SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(),
                           color.x,
                           color.y,
                           color.z,
                           color.w);
    SDL_RenderDrawPoints(gf2d_graphics_get_renderer(),pointArray,i);
    SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(),
                            255,
                            255,
                            255,
                            255);
    free(pointArray);
}


PointL * gf2d_draw_rectangle(int w,int h,int x_pos, int y_pos)
{
  SDL_Point * pointArray;
  PointL * pl;
  int size,area;
  size = 2*w+2*h;
  area = w*h;
  pointArray = (SDL_Point *)malloc(sizeof(SDL_Point)*size);
  int i,count;
  count = 0;
  for(i = x_pos; i <x_pos+w;i++)
  {
    pointArray[count].y = y_pos;
    pointArray[count].x = i;
    count = count+1;
    pointArray[count].y = y_pos+h-1;
    pointArray[count].x = i;
    count = count +1;
  }
  for(i = y_pos;i<y_pos+h;i++)
  {
      pointArray[count].y = i;
      pointArray[count].x = x_pos;
      count = count +1;
      pointArray[count].y = i;
      pointArray[count].x = x_pos+w;
      count = count +1;
  }
  pl = (PointL *) malloc(sizeof(PointL));
  pl->pointList = pointArray;
  pl->size = area;
  pl->count = count-1;
  return pl;

}

PointL * gf2d_draw_fill_polygon(PointL *p,int area)
{
  SDL_Point *fillArray,*pointArray;
  PointL * pl;
  int i,j,count,size;
  int * x_max,*x_min;

  pointArray =  p->pointList;
  size = p->count;
  x_max = (int *)malloc(sizeof(int)*2400);
  x_min = (int*)malloc(sizeof(int)*2400);
  memset(x_max,0,sizeof(int)*2400);
  memset(x_min,2400,sizeof(int)*2400);

  gf2d_insertion_sort_point(pointArray,size,x_max,x_min);
  int minY = pointArray[0].y;
  int maxY = pointArray[size-1].y;
  //slog("the min y is %d and the max y is %d",minY,maxY);
  fillArray = (SDL_Point*)malloc(sizeof(SDL_Point)*area);
  count = 0;
  for(i = minY;i<=maxY;i++)
  {
    for(j=x_min[i];j<x_max[i];j++)
    {
      fillArray[count].x = (int)j;
      fillArray[count].y = (int)i;
      count = count +1;
      //slog("fillArray x is %d fillArray y is %d",j,i);
      //slog("fillArray x is %d fillArray y is %d",fillArray[count-1].x,fillArray[count-1].y);
    }
  }

  free(x_max);
  free(x_min);
  pl = (PointL *) malloc(sizeof(PointL));
  pl->pointList = fillArray;
  pl->count = count -1;
  pl->size = area;
  //gf2d_draw_point_list_delete(p);
  return pl;
}

void gf2d_draw_point_list_delete(PointL *p)
{
  free(p->pointList);
  free(p);
}

void gf2d_draw_point_list(PointL * pointList)
{
  SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(),
                                                    0,
                                                    0,
                                                    0,
                                                    255);
SDL_RenderDrawPoints(gf2d_graphics_get_renderer(),pointList->pointList,pointList->count);
SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(),
                        255,
                        255,
                        255,
                        255);
}

int gf2d_draw_compare_points(void *point1,void *point2)
{
  return ((SDL_Point*)point1)->y > ((SDL_Point*)point2)->y;
}

/*eol@eof*/
