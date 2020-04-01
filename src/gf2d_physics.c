#include "gf2d_physics.h"


static PhysicsSpace s = {0};

void gf2d_physics_init()
{
  s.space = cpSpaceNew();
  cpSpaceSetIterations(s.space, 10);
	cpSpaceSetSleepTimeThreshold(s.space, 1000.0);
  //cpSpaceSetDamping(s.space,.1);
  gf2d_collision_handlers_add_all(s.space);
  atexit(gf2d_physics_close);
}

void gf2d_physics_update()
{
  float dt = 1.0/60.0;
  cpSpaceStep(s.space,dt);
}

cpShape * gf2d_physics_add_square_body(cpFloat length,cpFloat width,cpFloat radius,uint8_t type)
{
  cpBody *body;
  cpShape * shape;
  cpBB bb;
  bb.l = 0;
  bb.b = 0;
  bb.r = width;
  bb.t = length;
  if(type == 1)
  {//body that does not respond to physics and generally does not move
    body = cpSpaceAddBody(s.space,cpBodyNewStatic());
    //shape = cpSpaceAddShape(s.space,cpBoxShapeNew(body,width,length,radius));
    shape = cpSpaceAddShape(s.space,cpBoxShapeNew2(body,bb,radius));
    return shape;
  }
  else if(type == 0)
  {//body that is controlled by code not physics
    body = cpSpaceAddBody(s.space,cpBodyNewKinematic());
    //shape = cpSpaceAddShape(s.space,cpBoxShapeNew(body,width,length,radius));
    shape = cpSpaceAddShape(s.space,cpBoxShapeNew2(body,bb,radius));

    return shape;
  }
  else
  {//body that is controlled by physics
    body = cpSpaceAddBody(s.space,cpBodyNew(10,10));
//  shape = cpSpaceAddShape(s.space,cpBoxShapeNew(body,length,width,radius));
    shape = cpSpaceAddShape(s.space,cpBoxShapeNew2(body,bb,radius));
    return shape;
  }

}

void gf2d_physis_add_circle_body( )
{

}
static void ShapeFreeWrap(cpSpace *space, cpShape *shape, void *unused){
	cpSpaceRemoveShape(space, shape);
	cpShapeFree(shape);
}

static void PostShapeFree(cpShape *shape, cpSpace *space){
	cpSpaceAddPostStepCallback(space, (cpPostStepFunc)ShapeFreeWrap, shape, NULL);
}

static void ConstraintFreeWrap(cpSpace *space, cpConstraint *constraint, void *unused){
	cpSpaceRemoveConstraint(space, constraint);
	cpConstraintFree(constraint);
}

static void PostConstraintFree(cpConstraint *constraint, cpSpace *space){
	cpSpaceAddPostStepCallback(space, (cpPostStepFunc)ConstraintFreeWrap, constraint, NULL);
}

static void BodyFreeWrap(cpSpace *space, cpBody *body, void *unused){
	cpSpaceRemoveBody(space, body);
	cpBodyFree(body);
}

static void PostBodyFree(cpBody *body, cpSpace *space){
	cpSpaceAddPostStepCallback(space, (cpPostStepFunc)BodyFreeWrap, body, NULL);
}

cpSpace * gf2d_physics_get_space()
{
    return s.space;
}

void gf2d_physics_close()
{
  //ChipmunkDemoFreeSpaceChildren(s.space);
  cpSpaceEachShape(s.space, (cpSpaceShapeIteratorFunc)PostShapeFree, s.space);
	cpSpaceEachConstraint(s.space, (cpSpaceConstraintIteratorFunc)PostConstraintFree, s.space);
	cpSpaceEachBody(s.space, (cpSpaceBodyIteratorFunc)PostBodyFree, s.space);
  cpSpaceFree(s.space);
}
