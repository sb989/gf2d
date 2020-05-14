#ifndef __GF2D_ENTITY_H_
#define __GF2D_ENTITY_H_
#include "gf2d_sprite.h"
#include "gfc_vector.h"
#include "simple_logger.h"
#include "../Chipmunk-7.0.3/include/chipmunk/chipmunk.h"
#include "gf2d_physics.h"
#include "gfc_list.h"
typedef struct Entity_S{
  char * name;
  int hp;
  Sprite * s;
  Uint8 _inuse;
  float frame,maxFrame;
  cpBody * body;
  cpShape * shape;
  Vector4D * colorShift;
  Vector2D scale;
  void (*update)(void * self);
  void *updateData;
  void (*animate)(void *self);
  void *animateData;
  Vector2D position;
  int colliding;
  int knockback;
}Entity;

void gf2d_entity_animate(void *ent);
void gf2d_entity_draw(void *ent);
void gf2d_entity_update(Entity * e);
Entity * gf2d_entity_new(char * name, Sprite *s,Vector2D pos,uint32_t CollisionType,cpShapeFilter filter,Vector2D scale,Vector2D collisionBoxDim);
void gf2d_entity_manager_insert(Entity *e);
void gf2d_entity_update_all();
void gf2d_entity_update_stationary(void *ent);
void gf2d_entity_init_manager();
Entity * gf2d_entity_setup_collision_body(Entity *e,int length,int width,int radius, int type,uint32_t CollisionType,cpShapeFilter filter, Vector2D p);
void gf2d_entity_close();
void gf2d_entity_close_entity(Entity *e);
void gf2d_entity_animate_all();
#endif
