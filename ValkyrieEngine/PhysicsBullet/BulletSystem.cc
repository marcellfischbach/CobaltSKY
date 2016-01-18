

#include <PhysicsBullet/BulletSystem.hh>
#include <PhysicsBullet/BulletDynamicCollider.hh>
#include <PhysicsBullet/BulletStaticCollider.hh>
#include <PhysicsBullet/BulletBody.hh>
#include <PhysicsBullet/BulletScene.hh>
#include <PhysicsBullet/BulletShape.hh>

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


IPhysicsBody *vkBulletSystem::CreateBody()
{
  vkBulletBody *body = new vkBulletBody();

  return body;
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

