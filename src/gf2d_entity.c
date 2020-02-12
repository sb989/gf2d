#include "gf2d_entity.h"

typedef struct
{
  Entity ** entList;
  Uint8 size;
  Uint8 count;
}EntityManager;

static EntityManager entManager = {0};

void gf2d_entity_init_manager()
{
  int i;
  entManager.entList = (Entity**) malloc(sizeof(Entity*)*100);
  for(i = 0;i<100;i++)
  {
    entManager.entList[i] = (Entity*) malloc(sizeof(Entity*)*100);
  }
  entManager.size = 100;
  entManager.count = 0;
}

Entity * gf2d_entity_new(char * name, Sprite *s,Vector2D pos)
{
  if(entManager.count == entManager.size)
  {
    slog("max entitities created");
    return NULL;
  }
  Entity * temp = (Entity *)malloc(sizeof(Entity));
  temp->name = name;
  temp->s = s;
  temp->position = pos;
  gf2d_entity_manager_insert(temp);
  return temp;
}

void gf2d_entity_update_all()
{
  int i;

  for(i = 0;i<entManager.count;i++)
  {
    gf2d_entity_update(entManager.entList[i]);
  }
}

void gf2d_entity_update(Entity * e)
{
  if(e == NULL)
  {
    slog("entity is null. cant update");
  }

  if(e->update !=NULL)
    e->update(e);
}

void gf2d_entity_manager_insert(Entity *e)
{
  entManager.entList[entManager.count] = e;
  entManager.count = entManager.count +1;
}

void gf2d_entity_animate(Entity *e)
{

  gf2d_sprite_draw(e->s,e->position,NULL,NULL,NULL,NULL,NULL,NULL,e->frame);
  e->frame +=.1;

}
