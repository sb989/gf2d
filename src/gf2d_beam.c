#include "gf2d_beam.h"
#include "gf2d_player.h"
#include "gf2d_lightning.h"

static List * beams = NULL;

void gf2d_beam_init()
{
  slog("creating new beams system");
  beams = gfc_list_new();
  atexit(gf2d_beam_close);
}

Beam * gf2d_beam_new()
{
  Beam * temp = (Beam*)malloc(sizeof(Beam));
  return temp;
}

Beam * gf2d_beam_init_beam(Sprite *s,uint8_t CollisionType,cpShapeFilter filter,int length,float resizex,float resizey,int maxFrame, int radius)
{
  int i,count;
  Beam * b = NULL;

  if(!beams)
  {
    gf2d_beam_init();
  }
  count = gfc_list_get_count(beams);
  for(i=0;i<count;i++)
  {
    b = gfc_list_get_nth(beams,i);
    if(b->_inuse == 0)
      break;
    else
      b = NULL;
  }
  if(!b)
  {
    b = gf2d_beam_new();
    b->s = s;
    b->shoot = 0;
    b->CollisionType = CollisionType;
    b->filter = filter;
    b->_inuse = 1;
    b->length = length;
    b->radius = radius;
    b->resizex = resizex;
    b->resizey = resizey;
    b->maxFrame = maxFrame;
    b->frame = 0;
    b->colorShift = NULL;
    b->rotation = NULL;
    b->offsetAngle = 0;
    b->position = gf2d_player_get_pos();
    beams = gfc_list_append(beams,b);
    slog("added new beam to list");
  }
  else
  {
    b->shoot = 0;
    b->_inuse = 1;
    b->s = s;
    b->CollisionType = CollisionType;
    b->filter = filter;
    b->length = length;
    b->radius = radius;
    b->resizex = resizex;
    b->resizey = resizey;
    b->maxFrame = maxFrame;
    b->frame = 0;
    b->colorShift = NULL;
    b->rotation = NULL;
    b->offsetAngle = 0;
    b->position = gf2d_player_get_pos();
  }
  return b;
}

void gf2d_beam_animate_all()
{
  int i,count;
  Beam *b;
  count = gfc_list_get_count(beams);
  for(i =0;i<count;i++)
  {
    b = gfc_list_get_nth(beams,i);
    if(b->_inuse == 1)
    {
      gf2d_beam_animate(b);
    }
  }
}

void gf2d_beam_animate(Beam *beam)
{
  Vector2D * v2;
  //Vector4D * v4;
  Beam *b = (Beam*)beam;
  if(b->shoot ==0)
  {
    //slog("b->shoot is 0 returning");
    return;
  }
  //slog("b->shoot is 1");
  v2 = (Vector2D * )malloc(sizeof(Vector2D));
  v2->x = b->resizex;
  v2->y = b->resizey;

  gf2d_sprite_draw(b->s,b->position,v2,NULL,NULL,b->rotation,NULL,b->colorShift,(int)b->frame);
  if(b->frame < b->maxFrame)
    b->frame = b->frame + .2;
  else
    b->frame = 0;
  gf2d_draw_line(b->start,b->end, vector4d(255,0,0,255));
  gf2d_draw_circle(b->end,b->radius/4,vector4d(255,0,0,255));

  //slog("current frame is %f",b->ent->frame);
  free(v2);
}

void gf2d_beam_update_all()
{
  int i,count;
  Beam *b;
  count = gfc_list_get_count(beams);
  for(i =0;i<count;i++)
  {
    b = gfc_list_get_nth(beams,i);
    if(b->_inuse == 1)
    {
      gf2d_beam_update(b);
    }
  }
}

void gf2d_beam_update(Beam * b)
{
  Vector3D * rot;
  float angle;
  float y,x;

  Vector2D * normal;

  Vector2D playerPos = gf2d_player_get_pos();
  Vector2D mousePos = gf2d_mouse_pos();

  rot = (Vector3D*)malloc(sizeof(Vector3D));
  y = (mousePos.y - (playerPos.y+ .5*gf2d_player_get_draw_height()));
  x = (mousePos.x - (playerPos.x+ .5*gf2d_player_get_draw_width()));

  b->position.x = playerPos.x+ .5*gf2d_player_get_draw_width() + b->xoffset;
  b->position.y = playerPos.y+ .5*gf2d_player_get_draw_height() + b->yoffset;


  angle = (atan2(y,x) *180/M_PI);
  if(angle<0)
    angle = 360+angle;
  normal = (Vector2D*)malloc(sizeof(Vector2D));
  normal->x = x;
  normal->y = y;
  vector2d_normalize(normal);
  cpVect end;
  cpVect start;
  start.x = playerPos.x+ .5*gf2d_player_get_draw_width();
  start.y = playerPos.y+.5*gf2d_player_get_draw_height();
  end.x = start.x + normal->x *b->length;
  end.y = start.y + normal->y *b->length;

  b->start.x = start.x;
  b->start.y = start.y;
  b->end.x = end.x;
  b->end.y = end.y;
  cpSpaceSegmentQuery(
	gf2d_physics_get_space(),start,end,b->radius,
	b->filter,
	(cpSpaceSegmentQueryFunc)gf2d_beam_space_segment_query,b
  );

  rot->x =b->s->frame_w/2;
  rot->y =b->s->frame_h/1;
  rot->z = angle+b->offsetAngle;
  gf2d_beam_set_rotation(b,rot);
}

void gf2d_beam_space_segment_query(cpShape *shape, cpFloat t, cpVect n, void *data)
{
  cpCollisionType a;
  Beam *b = (Beam *)data;
  a = cpShapeGetCollisionType(shape);
  //slog("collision type is %d",a);
  if(a==ENEMIES)
  {
    gf2d_main_game_set_box_color(vector4d(255,0,0,255));
  }
  else
  {
    gf2d_main_game_set_box_color(vector4d(255,255,255,255));
  }
}

void gf2d_beam_set_rotation(Beam *b,Vector3D * rotation)
{
    b->rotation = rotation;
}

void gf2d_beam_set_colorShift(Beam *b, Vector4D * colorShift)
{
  b->colorShift = colorShift;
}

void gf2d_beam_close()
{
  int i,count;
  Beam * b;
  count = gfc_list_get_count(beams);
  slog("beam has %d memebers",count);
  for(i=0;i<count;i++)
  {
    b = gfc_list_get_nth(beams,i);
    if(b->colorShift)
      free(b->colorShift);
    if(b->rotation)
      free(b->rotation);
    free(b);
  }
  free(beams);
}
