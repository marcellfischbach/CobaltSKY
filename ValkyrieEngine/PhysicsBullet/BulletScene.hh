#pragma once

#include <PhysicsBullet/Export.hh>
#include <Valkyrie/Physics/IPhysicsScene.hh>
#include <PhysicsBullet/BulletScene.refl.hh>

VK_CLASS()
class VKBULLET_API vkBulletScene : public IPhysicsScene
{
  VK_CLASS_GEN_OBJECT;
public:
  vkBulletScene();
  virtual ~vkBulletScene();

};