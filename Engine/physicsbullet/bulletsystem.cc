

#include <physicsbullet/bulletsystem.hh>
#include <physicsbullet/bulletcapsulecharactercontroller.hh>
#include <physicsbullet/bulletdynamiccollider.hh>
#include <physicsbullet/bullethingejoint.hh>
#include <physicsbullet/bulletstaticcollider.hh>
#include <physicsbullet/bulletscene.hh>
#include <physicsbullet/bulletshape.hh>

cs::BulletSystem::BulletSystem()
  : cs::iPhysicsSystem ()
{

}


cs::BulletSystem::~BulletSystem()
{

}


void cs::BulletSystem::Initialize()
{

}


cs::iPhysicsScene *cs::BulletSystem::CreateScene()
{
  cs::BulletScene *scene = new cs::BulletScene(); 
  if (!scene->Initialize(false))
  {
    delete scene;
    scene = 0;
  }
  return scene;
}


cs::iPhysicsDynamicCollider *cs::BulletSystem::CreateDynamicCollider()
{
  cs::BulletDynamicCollider *dyn = new cs::BulletDynamicCollider();
  return dyn;
}

cs::iPhysicsStaticCollider *cs::BulletSystem::CreateStaticCollider()
{
  cs::BulletStaticCollider *stat = new cs::BulletStaticCollider();
  return stat;
}

cs::iPhysicsTriggerCollider *cs::BulletSystem::CreateTriggerCollider()
{
  return 0;
}


cs::iPhysicsShape *cs::BulletSystem::CreateShape(const cs::PhysGeometry &geometry)
{
  cs::BulletShape *shape = new cs::BulletShape();
  if (!shape->Initialize(geometry))
  {
    delete shape;
    shape = 0;
  }

  return shape;
}

cs::iPhysicsCapsuleCharacterController *cs::BulletSystem::CreateCapsulseCharacterController()
{
  cs::BulletCapsuleCharacterController *controller = new cs::BulletCapsuleCharacterController();
  return controller;
}


cs::iPhysicsJoint *cs::BulletSystem::CreateJoint(cs::ePhysicsJointType type, cs::iPhysicsDynamicCollider *colliderA, cs::iPhysicsDynamicCollider *colliderB)
{
  cs::BulletDynamicCollider *colA = cs::QueryClass<cs::BulletDynamicCollider>(colliderA);
  cs::BulletDynamicCollider *colB = cs::QueryClass<cs::BulletDynamicCollider>(colliderB);
  switch (type)
  {
  case cs::ePJT_Hinge:
    {
      cs::BulletHingeJoint *hingeJoint = new cs::BulletHingeJoint();
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