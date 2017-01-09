

#include <physicsbullet/bulletsystem.hh>
#include <physicsbullet/bulletcapsulecharactercontroller.hh>
#include <physicsbullet/bulletdynamiccollider.hh>
#include <physicsbullet/bullethingejoint.hh>
#include <physicsbullet/bulletstaticcollider.hh>
#include <physicsbullet/bulletscene.hh>
#include <physicsbullet/bulletshape.hh>

vkBulletSystem::vkBulletSystem()
  : iPhysicsSystem ()
{

}


vkBulletSystem::~vkBulletSystem()
{

}


void vkBulletSystem::Initialize()
{

}


iPhysicsScene *vkBulletSystem::CreateScene()
{
  vkBulletScene *scene = new vkBulletScene(); 
  if (!scene->Initialize(false))
  {
    delete scene;
    scene = 0;
  }
  return scene;
}


iPhysicsDynamicCollider *vkBulletSystem::CreateDynamicCollider()
{
  vkBulletDynamicCollider *dyn = new vkBulletDynamicCollider();
  return dyn;
}

iPhysicsStaticCollider *vkBulletSystem::CreateStaticCollider()
{
  vkBulletStaticCollider *stat = new vkBulletStaticCollider();
  return stat;
}

iPhysicsTriggerCollider *vkBulletSystem::CreateTriggerCollider()
{
  return 0;
}


iPhysicsShape *vkBulletSystem::CreateShape(const vkPhysGeometry &geometry)
{
  vkBulletShape *shape = new vkBulletShape();
  if (!shape->Initialize(geometry))
  {
    delete shape;
    shape = 0;
  }

  return shape;
}

iPhysicsCapsuleCharacterController *vkBulletSystem::CreateCapsulseCharacterController()
{
  vkBulletCapsuleCharacterController *controller = new vkBulletCapsuleCharacterController();
  return controller;
}


iPhysicsJoint *vkBulletSystem::CreateJoint(vkPhysicsJointType type, iPhysicsDynamicCollider *colliderA, iPhysicsDynamicCollider *colliderB)
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