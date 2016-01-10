#pragma once

#include <PhysicsBullet/Export.hh>
#include <Valkyrie/Physics/IPhysicsBody.hh>
#include <PhysicsBullet/BulletBody.refl.hh>

VK_CLASS()
class VKBULLET_API vkBulletBody : public IPhysicsBody
{
  VK_CLASS_GEN_OBJECT;
public:
  vkBulletBody();
  virtual ~vkBulletBody();

};