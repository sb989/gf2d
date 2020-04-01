#include "gf2d_projectile.h"


List * projectiles = NULL;

void gf2d_projectile_init()
{
  projectiles = gfc_list_new();
  atexit(gf2d_projectile_close);
}

Projectile * gf2d_projectile_init_projectile(Sprite *s,uint32_t CollisionType,cpShapeFilter filter,float resizex,float resizey,int maxFrame,int owner,Vector2D pos,float range)
{
  int i,count;
  Projectile * proj = NULL;
  Entity *e;
  if(!projectiles)
    gf2d_projectile_init();
  count = gfc_list_get_count(projectiles);
  for(i=0;i<count;i++)
  {
    proj = gfc_list_get_nth(projectiles,i);
    if(proj->_inuse == 0)
      break;
    else
      proj = NULL;
  }
   if(!proj)
   {
     proj = gf2d_projectile_new();
     e = gf2d_entity_new("Projectile",s,pos,CollisionType,filter,vector2d(resizex,resizey),vector2d(s->frame_w,s->frame_h*resizey));
     //slog("entity position is %f %f",pos.x,pos.y);
     proj->_inuse = 1;
     proj->ent = e;
     proj->resizex = resizex;
     proj->resizey = resizey;
     proj->maxFrame = maxFrame;
     proj->ent->frame = 0;
     proj->colorShift = NULL;
     proj->ent->updateData = proj;
     proj->ent->animateData = proj;
     proj->offsetAngle = 0;
     //proj->velocity = vel;
     proj->rotation = NULL;
     proj->xoffset = 0;
     proj->yoffset = 0;
     proj->destroy = 0;
     proj->owner = owner;
     proj->destStart = maxFrame;
     proj->range = range;
     proj->shadowFrame = -1;
     proj->startFrame = 0;
     //proj->originalPos = originalPos;
     projectiles = gfc_list_append(projectiles,proj);
   }
   else
   {
     e = gf2d_entity_new("Projectile",s,pos,CollisionType,filter,vector2d(resizex,resizey),vector2d(s->frame_w,s->frame_h*resizey));
     //slog("entity position is %f %f",pos.x,pos.y);
     proj->_inuse = 1;
     proj->ent = e;
     proj->resizex = resizex;
     proj->resizey = resizey;
     proj->maxFrame = maxFrame;
     proj->ent->frame = 0;
     proj->colorShift = NULL;
     proj->ent->updateData = proj;
     proj->ent->animateData = proj;
     proj->offsetAngle = 0;
     //proj->velocity = vel;
     proj->rotation = NULL;
     proj->xoffset = 0;
     proj->yoffset = 0;
     proj->owner = owner;
     proj->destroy = 0;
     proj->destStart = maxFrame;
     proj->range = range;
     proj->shadowFrame = -1;
     proj->startFrame = 0;

   }

  return proj;
}

Projectile * gf2d_projectile_new()
{
  Projectile * proj = (Projectile *)malloc(sizeof(Projectile));
  //proj->_inuse = 0;
  return proj;
}

Projectile * gf2d_projectile_get_proj_from_ent(Entity *e)
{
  int i,count;
  Projectile * proj;
  count = gfc_list_get_count(projectiles);
  for(i=0;i<count;i++)
  {
    proj = gfc_list_get_nth(projectiles,i);
    if(proj->ent == e)
      return proj;
  }
  return NULL;
}

void gf2d_projectile_animate(void *projectile)
{
  Vector2D * v2;
  Projectile *proj = (Projectile *)projectile;
  Vector2D draw;
  if(proj->ent->_inuse == 0)
  {
    //slog("not drawing returning instead");
    return;

  }
  //Vector4D * v4;
  v2 = (Vector2D * )malloc(sizeof(Vector2D));
  //slog("draw projectile");
  v2->x = proj->resizex;
  v2->y = proj->resizey;
  draw.x = proj->ent->position.x + proj->xoffset;
  draw.y = proj->ent->position.y + proj->yoffset;
  //slog("draw pos is %f %f",draw.x,draw.y);
  //slog("player pos is %f %f",gf2d_player_get_pos().x, gf2d_player_get_pos().y);
  if(proj->shadowFrame > -1)
  {
    gf2d_sprite_draw(proj->ent->s,draw,v2,NULL,NULL,proj->rotation,NULL,proj->colorShift,(int)proj->shadowFrame);

  }
  gf2d_sprite_draw(proj->ent->s,draw,v2,NULL,NULL,proj->rotation,NULL,proj->colorShift,(int)proj->ent->frame);
  //slog("just drew frame %f",proj->ent->frame);

  if(proj->destroy != 1 && proj->ent->frame < proj->destStart-1)
    proj->ent->frame = proj->ent->frame + .05;
  else if(proj->destroy != 1 && proj->ent->frame >= proj->destStart-1)
  {
    proj->ent->frame = proj->startFrame;
  }
  else if(proj->destroy ==1 && proj->ent->frame < proj->maxFrame-1)
  {
    proj->ent->frame = proj->ent->frame + .1;
  }
  else if(proj->destroy == 1 && proj->ent->frame >= proj->maxFrame-1)
  {

    proj->_inuse = 0;
    proj->ent->_inuse = 0;
    proj->count(proj);
    proj->destroy = 0;
    //slog("done drawing");
  }
  free(v2);
}

void gf2d_projectile_update(void * projectile)
{
  cpVect pos;
  float magnitude;
  Projectile *proj = (Projectile*)projectile;
  if(proj->ent->_inuse == 0 || proj->destroy  ==1)
  {
    //slog("ent inuse is 0  or destroy is 1; returning");
    return;
  }

//  slog("current pos before update is %f %f",proj->ent->position.x,proj->ent->position.y);
  pos = cpBodyGetPosition(proj->ent->body);
  proj->ent->position.x = pos.x;
  proj->ent->position.y = pos.y;
  magnitude = vector2d_magnitude_between(proj->ent->position,proj->originalPos);
//  slog("original pos is %f %f",proj->originalPos.x,proj->originalPos.y);
//  slog("current pos after update is %f %f",proj->ent->position.x,proj->ent->position.y);
  //slog("distance between start and current is %f",magnitude);
  if(magnitude > proj->range)
  {
    gf2d_projectile_destroy(proj);
  }
}

void gf2d_projectile_shoot(float speed,Projectile *proj)
{
  Vector3D * rot;
  float angle;
  float y,x;
  Vector2D * normal;
  cpVect velocity;
  Vector2D playerPos = gf2d_player_get_pos();
  Vector2D mousePos = gf2d_mouse_pos();

  rot = (Vector3D*)malloc(sizeof(Vector3D));

  y = (mousePos.y - (playerPos.y+ .5*gf2d_player_get_draw_height()));
  x = (mousePos.x - (playerPos.x+ .5*gf2d_player_get_draw_width()));

  proj->ent->position.x = playerPos.x + proj->xoffset+ .5*gf2d_player_get_draw_width();
  proj->ent->position.y = playerPos.y + proj->yoffset+ .5*gf2d_player_get_draw_height();
  proj->ent->_inuse = 1;
  angle = (atan2(y,x) *180/M_PI);
  if(angle<0)
    angle = 360+angle;

  rot->x =proj->ent->s->frame_w/2;
  rot->y =proj->ent->s->frame_h/1;
  rot->z = angle + proj->offsetAngle;
  gf2d_projectile_set_rotation(proj,rot);

  normal = (Vector2D*)malloc(sizeof(Vector2D));
  normal->x = x;
  normal->y = y;
  vector2d_normalize(normal);

  velocity.x = normal->x *speed;
  velocity.y = normal->y *speed;
  cpBodySetVelocity(proj->ent->body,velocity);
}

void gf2d_projectile_destroy(Projectile *proj)
{
  cpVect velocity;
  velocity.x = 0;
  velocity.y = 0;
  cpBodySetVelocity(proj->ent->body,velocity);
  proj->destroy = 1;

}

void gf2d_projectile_set_rotation(Projectile *proj,Vector3D * rotation)
{
    proj->rotation = rotation;
}

void gf2d_projectile_set_colorShift(Projectile *proj, Vector4D * colorShift)
{
  proj->colorShift = colorShift;
}


void gf2d_projectile_close()
{
  int i,count;
  Projectile * proj;
  count = gfc_list_get_count(projectiles);
  for(i=0;i<count;i++)
  {
    proj = gfc_list_get_nth(projectiles,i);
    free(proj);
  }
  free(projectiles);
}
