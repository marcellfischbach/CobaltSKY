

#include <PhysicsBullet/BulletSystem.hh>
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

