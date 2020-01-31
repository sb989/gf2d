#ifndef __GF2D_DRAW_H__
#define __GF2D_DRAW_H__

#include <SDL.h>
#include "gfc_vector.h"

typedef struct
{
  SDL_Point * pointList;
  int count;
  int size;
}PointL;


/**
 * @brief draw a pixel to the current rendering context
 * @param pixel the screen position to draw at
 * @param color the color to draw with
 */
void gf2d_draw_pixel(Vector2D pixel,Vector4D color);

/**
* @brief generate PointL for the perimeter of the rectangle
* @param w width of the rectangle
* @param h height of the rectangle
* @param x_pos starting x position of the rectangle
* @param y_pos starting y position of the rectangle
*/
PointL * gf2d_draw_rectangle(int w,int h,int x_pos, int y_pos);

/**
* @brief generate a PointL for the inside of the polygon
* @param  p    PointL containing the points of the polygons perimeter
* @param  area the area of the polygon
* @return      PointL containing the points of the inside of the polygon
*/
PointL * gf2d_draw_fill_polygon(PointL *p,int area);

void gf2d_draw_circle(Vector2D center, int radius, Vector4D color);
void gf2d_draw_rect(SDL_Rect rect,Vector4D color);
void gf2d_draw_fill_rect(SDL_Rect rect,Vector4D color);

/**
* @brief draws the points in the PointL
* @param pointList PointL containing the points to draw
*/
void gf2d_draw_point_list(PointL * pointList);


/**
 * @brief draw a line to the current rendering context
 * @param p1 starting point
 * @param p2 ending point
 * @param color the color to draw with
 */
void gf2d_draw_line(Vector2D p1,Vector2D p2, Vector4D color);

#endif
