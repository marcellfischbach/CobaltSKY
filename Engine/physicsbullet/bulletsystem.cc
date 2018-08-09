

#include <physicsbullet/bulletsystem.hh>
#include <physicsbullet/bulletcapsulecharactercontroller.hh>
#include <physicsbullet/bulletdynamiccollider.hh>
#include <physicsbullet/bullethingejoint.hh>
#include <physicsbullet/bulletstaticcollider.hh>
#include <physicsbullet/bulletscene.hh>
#include <physicsbullet/bulletshape.hh>

csBulletSystem::csBulletSystem()
  : iPhysicsSystem ()
{

}


csBulletSystem::~csBulletSystem()
{

}


void csBulletSystem::Initialize()
{

}


iPhysicsScene *csBulletSystem::CreateScene()
{
  csBulletScene *scene = new csBulletScene(); 
  if (!scene->Initialize(false))
  {
    delete scene;
    scene = 0;
  }
  return scene;
}


iPhysicsDynamicCollider *csBulletSystem::CreateDynamicCollider()
{
  csBulletDynamicCollider *dyn = new csBulletDynamicCollider();
  return dyn;
}

iPhysicsStaticCollider *csBulletSystem::CreateStaticCollider()
{
  csBulletStaticCollider *stat = new csBulletStaticCollider();
  return stat;
}

iPhysicsTriggerCollider *csBulletSystem::CreateTriggerCollider()
{
  return 0;
}


iPhysicsShape *csBulletSystem::CreateShape(const csPhysGeometry &geometry)
{
  csBulletShape *shape = new csBulletShape();
  if (!shape->Initialize(geometry))
  {
    delete shape;
    shape = 0;
  }

  return shape;
}

iPhysicsCapsuleCharacterController *csBulletSystem::CreateCapsulseCharacterController()
{
  csBulletCapsuleCharacterController *controller = new csBulletCapsuleCharacterController();
  return controller;
}


iPhysicsJoint *csBulletSystem::CreateJoint(csPhysicsJointType type, iPhysicsDynamicCollider *colliderA, iPhysicsDynamicCollider *colliderB)
{
  csBulletDynamicCollider *colA = csQueryClass<csBulletDynamicCollider>(colliderA);
  csBulletDynamicCollider *colB = csQueryClass<csBulletDynamicCollider>(colliderB);
  switch (type)
  {
  case ePJT_Hinge:
    {
      csBulletHingeJoint *hingeJoint = new csBulletHingeJoint();
      if (!hingeJoint->Initialize(colA, colB))
      {
        delete hingeJoint;
        hingeJoint = 0;
      }
      return hingeJoint;
    }
  }

  return 0;
}