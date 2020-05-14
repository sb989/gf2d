#include "gf2d_beam.h"
#include "gf2d_player.h"
#include "gf2d_lightning.h"
#include "gf2d_enemy.h"

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

Beam * gf2d_beam_init_beam(Sprite *s,uint32_t CollisionType,cpShapeFilter filter,int length,float resizex,float resizey,int maxFrame, int radius)
{
  int i,count;
  Beam * b = NULL;

  if(!beams)
  {
    gf2d_beam_init();
    slog("created beams list");
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
  else if(b)
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
    slog("returning an old beam");
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

  Vector2D s_top,s_bot,e_top,e_bot;
  cpVect n;
  n.x = b->start.x - b->end.x;
  n.y = b->start.y - b->end.y;
  n = cpvnormalize(n);

  s_top.x = -b->radius*n.y + b->start.x;
  s_top.y = b->radius*n.x + b->start.y;

  s_bot.x = b->radius*n.y + b->start.x;
  s_bot.y = -b->radius*n.x + b->start.y;

  e_top.x = s_top.x + n.x*-b->length;
  e_top.y = s_top.y + n.y*-b->length;

  e_bot.x = s_bot.x + n.x*-b->length;
  e_bot.y = s_bot.y + n.y*-b->length;

  gf2d_draw_line(b->start,s_top, vector4d(255,0,0,255));
  gf2d_draw_line(b->start,s_bot, vector4d(255,0,0,255));
  gf2d_draw_line(s_top,e_top,vector4d(255,0,0,255));
  gf2d_draw_line(s_bot,e_bot,vector4d(255,0,0,255));


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
  //Vector3D * rot;
  float angle;
  float y,x;
  cpVect s_top,s_bot,e_top,e_bot;
  Vector2D * normal;
  if(b->shoot == 0)
    return;
  Vector2D playerPos = gf2d_player_get_pos();
  Vector2D mousePos = gf2d_mouse_pos();

  //rot = (Vector3D*)malloc(sizeof(Vector3D));
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


  s_top.x = -b->radius*normal->y + b->start.x;
  s_top.y = b->radius*normal->x + b->start.y;

  s_bot.x = b->radius*normal->y + b->start.x;
  s_bot.y = -b->radius*normal->x + b->start.y;

  e_top.x = s_top.x + normal->x*-b->length;
  e_top.y = s_top.y + normal->y*-b->length;

  e_bot.x = s_bot.x + normal->x*-b->length;
  e_bot.y = s_bot.y + normal->y*-b->length;

  cpSpaceSegmentQuery(
	gf2d_physics_get_space(),s_top,e_top,b->radius,
	b->filter,
	(cpSpaceSegmentQueryFunc)gf2d_beam_space_segment_query,b
  );

  cpSpaceSegmentQuery(
	gf2d_physics_get_space(),s_bot,e_bot,b->radius,
	b->filter,
	(cpSpaceSegmentQueryFunc)gf2d_beam_space_segment_query,b
  );
  if(b->rotation == NULL)
  {
    b->rotation = (Vector3D*)malloc(sizeof(Vector3D));
  }

  b->rotation->x =b->s->frame_w/2;
  b->rotation->y =b->s->frame_h/1;
  b->rotation->z = angle+b->offsetAngle;
  free(normal);
  //gf2d_beam_set_rotation(b,rot);
}

void gf2d_beam_space_segment_query(cpShape *shape, cpFloat t, cpVect n, void *data)
{
  cpCollisionType a;
  Beam *b = (Beam *)data;
  a = cpShapeGetCollisionType(shape);
  if(b->CollisionType == WATER)
  {
    //slog("i am water ");
  }
  if(a==ENEMIES && (b->CollisionType== FIRE || b->CollisionType== LIGHTNING || b->CollisionType == WATER))
  {
    //slog("fire or lightning");
    //gf2d_main_game_set_box_color(vector4d(255,0,0,255));
    cpBody *enemyBody = cpShapeGetBody(shape);
    Entity *enemy = (Entity *)cpBodyGetUserData(enemyBody);
    gf2d_enemy_take_damage(1,enemy);
    cpSpaceAddPostStepCallback(gf2d_physics_get_space(), (cpPostStepFunc)gf2d_enemy_drop_coin,enemy, NULL);
  }
  else if(a==ENEMIES && b->CollisionType == WIND)
  {
    cpVect vel;
    //slog("n is %f %f",n.x,n.y);

    //gf2d_main_game_set_box_color(vector4d(255,0,0,255));
    cpBody *enemyBody = cpShapeGetBody(shape);
    Entity *enemy = (Entity *)cpBodyGetUserData(enemyBody);
    if(enemy->knockback == 1)
      return;
    enemy->knockback = 1;
    vel = cpvnormalize(cpBodyGetVelocity(enemyBody));
    //slog("enemy velocity is %f %f",cpBodyGetVelocity(enemyBody).x,cpBodyGetVelocity(enemyBody).y);
    vel = cpvneg(vel);
    //slog("negated normalized enemy velocity is %f %f",vel.x,vel.y);
    vel = cpvmult(vel,2000);
    //slog("vel is %f %f",vel.x, vel.y);
    cpBodySetVelocity(enemy->body,vel);
    gf2d_enemy_take_damage(.2,enemy);
    cpSpaceAddPostStepCallback(gf2d_physics_get_space(), (cpPostStepFunc)gf2d_enemy_drop_coin,enemy, NULL);
  }
  else
  {
    //slog("i am nothing");
    //slog("nothings number is %u",b->CollisionType);
    //gf2d_main_game_set_box_color(vector4d(255,255,255,255));
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
