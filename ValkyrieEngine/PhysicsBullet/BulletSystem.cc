

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
  return new vkBulletScene(); 
}

