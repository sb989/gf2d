#ifndef __GF2D_PROJECTILE_H_
#define __GF2D_PROJECTILE_H_
#include "gfc_list.h"
#include "gf2d_entity.h"
#include "gf2d_player.h"
typedef struct
{
  int _inuse;
  Entity *ent;
  float resizex,resizey;
  int maxFrame;
  Vector4D *colorShift;
  float offsetAngle;
  Vector2D velocity;
  Vector2D originalPos;
  Vector3D * rotation;
  float xoffset,yoffset;
  int owner;
}Projectile;

void gf2d_projectile_init();
Projectile * gf2d_projectile_init_projectile(Sprite *s,uint8_t CollisionType,cpShapeFilter filter,float resizex,float resizey,int maxFrame,int player,Vector2D pos);
Projectile * gf2d_projectile_new();
void gf2d_projectile_animate(void *proj);
void gf2d_projectile_update(void * projectile);
void gf2d_projectile_set_rotation(Projectile *proj,Vector3D * rotation);
void gf2d_projectile_set_colorShift(Projectile *proj, Vector4D * colorShift);
void gf2d_projectile_shoot(float speed,Projectile *proj);
void gf2d_projectile_destroy(Projectile *proj);
void gf2d_projectile_close();

#endif
