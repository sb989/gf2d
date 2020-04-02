#include "gf2d_entity.h"

typedef struct
{
  List * entList;
  Uint8 size;
  Uint8 count;
}EntityManager;

static EntityManager entManager = {0};

void gf2d_entity_init_manager()
{
  entManager.entList = gfc_list_new();
  entManager.size = 100;
  entManager.count = 0;
  atexit(gf2d_entity_close);
}

Entity * gf2d_entity_new(char * name, Sprite *s,Vector2D pos, uint32_t CollisionType,cpShapeFilter filter,Vector2D scale,Vector2D collisionBoxDim)
{
  if(entManager.count == entManager.size)
  {
    slog("max entitities created");
    return NULL;
  }
  int i,count;
  Entity * temp = NULL;
  count = gfc_list_get_count(entManager.entList);
  for(i = 0;i<count;i++)
  {
    temp = gfc_list_get_nth(entManager.entList,i);
    if(temp->_inuse == 1)
      temp = NULL;
    else
    {
      slog("entitys name is %s",temp->name);
    }
      break;
  }
  if(temp == NULL)
  {
    temp = (Entity *)malloc(sizeof(Entity));
  }
  else//players inuse is set to 0 in the beginning so this needs to be fixed
  {// because it is currenlty delting players body and shape
    cpSpace * space = gf2d_physics_get_space();
    cpSpaceRemoveShape(space, temp->shape);
    cpSpaceRemoveBody(space, temp->body);
    cpShapeFree(temp->shape);
    cpBodyFree(temp->body);
  }
  //Uint8 CollisionType = 0;
  temp->colorShift = NULL;
  temp->name = name;
  temp->s = s;
  temp->hp = 10;
  temp->position = pos;
  temp->_inuse = 0;
  temp->frame = 0;
  temp->maxFrame =1;
  temp->update = NULL;
  temp->updateData = temp;
  temp->animate = &gf2d_entity_animate;
  temp->animateData = temp;
  temp->scale = scale;
  temp->colliding = 0;
  temp->knockback = 0;
  cpFloat length,width,radius;
  length = s->frame_h;
  width = s->frame_w;
  radius = 0;
  temp = gf2d_entity_setup_collision_body(temp,collisionBoxDim.y,collisionBoxDim.x,radius,0,CollisionType,filter,pos);

  entManager.entList = gfc_list_append(entManager.entList,temp);
  return temp;
}

Entity * gf2d_entity_setup_collision_body(Entity *e,int length,int width,int radius, int type,uint32_t CollisionType,cpShapeFilter filter,Vector2D p)
{
  cpVect pos;
  pos.x = p.x;
  pos.y = p.y;
  e->shape = gf2d_physics_add_square_body(length,width,radius,type);
  e->body = cpShapeGetBody(e->shape);
  cpShapeSetCollisionType(e->shape,CollisionType);
  cpBodySetUserData(e->body,e);
  cpShapeSetFilter(e->shape,filter);
  cpBodySetPosition(e->body,pos);
  cpSpaceReindexShapesForBody(gf2d_physics_get_space(),e->body);
  //gf2d_entity_manager_insert(e);
  return e;
}

void gf2d_entity_update_all()
{
  int i,count;
  Entity *e;
  count = gfc_list_get_count(entManager.entList);
  //slog("count is %d",count);
  for(i = 0;i<count;i++)
  {
    e = gfc_list_get_nth(entManager.entList,i);
    //slog("es name is %s",e->name);
    gf2d_entity_update(e);
  }
}

void gf2d_entity_update(Entity * e)
{
  if(e == NULL)
  {
    slog("entity is null. cant update");
  }

  if(e->update !=NULL)
    e->update(e->updateData);
}

void gf2d_entity_animate_all()
{
  int i,count;
  Entity *e;
  count = gfc_list_get_count(entManager.entList);
  //slog("animating %d entities",count);
  for(i = 0;i<count;i++)
  {
    //slog("animating entity %d",i);
    e = gfc_list_get_nth(entManager.entList,i);
    if(e->animate !=NULL)
      e->animate(e->animateData);
  }
}

void gf2d_entity_animate(void *ent)
{
  Entity *e = (Entity *)ent;
  if(e->_inuse == 0)
    return;
  gf2d_entity_draw(e);
  if(e->frame + .1< e->maxFrame)
    e->frame +=.1;
  else
    e->frame = 0;
}

void gf2d_entity_draw(void *ent)
{
  Entity * e= (Entity*)ent;
  if(e->_inuse == 0)
    return;
  gf2d_sprite_draw(e->s,e->position,&(e->scale),NULL,NULL,NULL,NULL,e->colorShift,e->frame);//e->frame);
}

void gf2d_entity_close()
{
  int i,count;
  Entity *e;
  count = gfc_list_get_count(entManager.entList);
  for(i=0;i<count;i++)
  {
    e = gfc_list_get_nth(entManager.entList,i);
    gf2d_entity_close_entity(e);
    free(e);
  }
}

void gf2d_entity_close_entity(Entity *e)
{
  cpSpace * space = gf2d_physics_get_space();
  cpSpaceRemoveShape(space, e->shape);
  cpSpaceRemoveBody(space, e->body);
  cpShapeFree(e->shape);
  cpBodyFree(e->body);
//  if(e->colorShift != NULL)
  //  free(e->colorShift);
  //if(e->scale)
    //free(e->scale);
}
