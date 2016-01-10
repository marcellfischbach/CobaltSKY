#pragma once

#include <PhysicsBullet/Export.hh>
#include <Valkyrie/Physics/IPhysicsShape.hh>
#include <PhysicsBullet/BulletShape.refl.hh>

VK_CLASS()
class VKBULLET_API vkBulletShape : public IPhysicsShape
{
  VK_CLASS_GEN_OBJECT;
public:
  vkBulletShape();
  virtual ~vkBulletShape();

};