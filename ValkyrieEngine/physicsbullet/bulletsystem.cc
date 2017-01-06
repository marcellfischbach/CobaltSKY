

#include <physicsbullet/bulletsystem.hh>
#include <physicsbullet/bulletcapsulecharactercontroller.hh>
#include <physicsbullet/bulletdynamiccollider.hh>
#include <physicsbullet/bullethingejoint.hh>
#include <physicsbullet/bulletstaticcollider.hh>
#include <physicsbullet/bulletscene.hh>
#include <physicsbullet/bulletshape.hh>

vkBulletSystem::vkBulletSystem()
  : IPhysicsSystem ()
{

}


vkBulletSystem::~vkBulletSystem()
{

}


void vkBulletSystem::Initialize()
{

}


IPhysicsScene *vkBulletSystem::CreateScene()
{
  vkBulletScene *scene = new vkBulletScene(); 
  if (!scene->Initialize(false))
  {
    delete scene;
    scene = 0;
  }
  return scene;
}


IPhysicsDynamicCollider *vkBulletSystem::CreateDynamicCollider()
{
  vkBulletDynamicCollider *dyn = new vkBulletDynamicCollider();
  return dyn;
}

IPhysicsStaticCollider *vkBulletSystem::CreateStaticCollider()
{
  vkBulletStaticCollider *stat = new vkBulletStaticCollider();
  return stat;
}

IPhysicsTriggerCollider *vkBulletSystem::CreateTriggerCollider()
{
  return 0;
}


IPhysicsShape *vkBulletSystem::CreateShape(const vkPhysGeometry &geometry)
{
  vkBulletShape *shape = new vkBulletShape();
  if (!shape->Initialize(geometry))
  {
    delete shape;
    shape = 0;
  }

  return shape;
}

IPhysicsCapsuleCharacterController *vkBulletSystem::CreateCapsulseCharacterController()
{
  vkBulletCapsuleCharacterController *controller = new vkBulletCapsuleCharacterController();
  return controller;
}


IPhysicsJoint *vkBulletSystem::CreateJoint(vkPhysicsJointType type, IPhysicsDynamicCollider *colliderA, IPhysicsDynamicCollider *colliderB)
{
  vkBulletDynamicCollider *colA = vkQueryClass<vkBulletDynamicCollider>(colliderA);
  vkBulletDynamicCollider *colB = vkQueryClass<vkBulletDynamicCollider>(colliderB);
  switch (type)
  {
  case ePJT_Hinge:
    {
      vkBulletHingeJoint *hingeJoint = new vkBulletHingeJoint();
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