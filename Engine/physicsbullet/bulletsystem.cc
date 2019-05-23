

#include <physicsbullet/bulletsystem.hh>
#include <physicsbullet/bulletcapsulecharactercontroller.hh>
#include <physicsbullet/bulletdynamiccollider.hh>
#include <physicsbullet/bullethingejoint.hh>
#include <physicsbullet/bulletstaticcollider.hh>
#include <physicsbullet/bulletscene.hh>
#include <physicsbullet/bulletshape.hh>

csBulletSystem::csBulletSystem()
  : cs::iPhysicsSystem ()
{

}


csBulletSystem::~csBulletSystem()
{

}


void csBulletSystem::Initialize()
{

}


cs::iPhysicsScene *csBulletSystem::CreateScene()
{
  csBulletScene *scene = new csBulletScene(); 
  if (!scene->Initialize(false))
  {
    delete scene;
    scene = 0;
  }
  return scene;
}


cs::iPhysicsDynamicCollider *csBulletSystem::CreateDynamicCollider()
{
  csBulletDynamicCollider *dyn = new csBulletDynamicCollider();
  return dyn;
}

cs::iPhysicsStaticCollider *csBulletSystem::CreateStaticCollider()
{
  csBulletStaticCollider *stat = new csBulletStaticCollider();
  return stat;
}

cs::iPhysicsTriggerCollider *csBulletSystem::CreateTriggerCollider()
{
  return 0;
}


cs::iPhysicsShape *csBulletSystem::CreateShape(const cs::PhysGeometry &geometry)
{
  csBulletShape *shape = new csBulletShape();
  if (!shape->Initialize(geometry))
  {
    delete shape;
    shape = 0;
  }

  return shape;
}

cs::iPhysicsCapsuleCharacterController *csBulletSystem::CreateCapsulseCharacterController()
{
  csBulletCapsuleCharacterController *controller = new csBulletCapsuleCharacterController();
  return controller;
}


cs::iPhysicsJoint *csBulletSystem::CreateJoint(cs::ePhysicsJointType type, cs::iPhysicsDynamicCollider *colliderA, cs::iPhysicsDynamicCollider *colliderB)
{
  csBulletDynamicCollider *colA = cs::QueryClass<csBulletDynamicCollider>(colliderA);
  csBulletDynamicCollider *colB = cs::QueryClass<csBulletDynamicCollider>(colliderB);
  switch (type)
  {
  case cs::ePJT_Hinge:
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