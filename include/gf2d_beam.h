#ifndef __GF2D_BEAM_H_
#define __GF2D_BEAM_H_
#include "gfc_list.h"
#include "gf2d_entity.h"

typedef struct
{
  Sprite *s;
  int length,radius;
  float resizex,resizey;
  int _inuse;
  int shoot;
  float frame;
  int maxFrame;
  float offsetAngle;
  Vector4D * colorShift;
  Vector3D * rotation;
  Vector2D start;
  Vector2D end;
  Vector2D position;
  float xoffset,yoffset;
  uint32_t CollisionType;
  cpShapeFilter filter;
}Beam;

void gf2d_beam_init();
Beam * gf2d_beam_new();
Beam * gf2d_beam_init_beam(Sprite *s,uint32_t CollisionType,cpShapeFilter filter,int length,float resizex,float resizey,int maxFrame, int radius);
void gf2d_beam_animate(Beam *b);
void gf2d_beam_animate_all();
void gf2d_beam_set_rotation(Beam *b,Vector3D * rotation);
void gf2d_beam_set_colorShift(Beam *b, Vector4D * colorShift);
void gf2d_beam_update(Beam * beam);
void gf2d_beam_update_all();
void gf2d_beam_close();
void gf2d_beam_space_segment_query(cpShape *shape, cpFloat t, cpVect n, void *data);
#endif
