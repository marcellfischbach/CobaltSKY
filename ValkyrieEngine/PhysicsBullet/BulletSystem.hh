
#pragma once

#include <PhysicsBullet/Export.hh>
#include <Valkyrie/Physics/IPhysicsSystem.hh>
#include <PhysicsBullet/BulletSystem.refl.hh>



VK_CLASS()
class VKBULLET_API vkBulletSystem : public IPhysicsSystem
{
  VK_CLASS_GEN_OBJECT;

public:
  vkBulletSystem();
  virtual ~vkBulletSystem();

  virtual void Initialize();

  virtual IPhysicsScene *CreateScene();

  virtual IPhysicsBody *CreateBody();

  virtual IPhysicsShape *CreateShape(const vkPhysGeometry &geometry);
};

