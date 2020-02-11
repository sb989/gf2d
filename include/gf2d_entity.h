#ifndef __GF2D_ENTITY_H_
#define __GF2D_ENTITY_H_
#include "gf2d_sprite.h"
#include "gfc_vector.h"
#include "simple_logger.h"

typedef struct Entity_S{
  char * name;
  Sprite * s;
  Uint8 _inuse;
  float frame;
  //void (*think) (struct Entity_S self);
  void (*update) ( struct Entity_S * self);
  Vector2D position;

}Entity;

void gf2d_entity_animate(Entity *e);
void gf2d_entity_update(Entity * e);
Entity * gf2d_entity_new(char * name, Sprite *s,Vector2D pos);
void gf2d_entity_manager_insert(Entity *e);
void gf2d_entity_update_all();
void gf2d_entity_init_manager();

#endif