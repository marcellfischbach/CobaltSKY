
#pragma once

#include <PhysicsBullet/Export.hh>
#include <Valkyrie/Physics/IPhysicsSystem.hh>
#include <PhysicsBullet/BulletSystem.refl.hh>



VK_CLASS()
class VKBULLET_API vkBulletSystem : public VK_SUPER(IPhysicsSystem)
{
  VK_CLASS_GEN_OBJECT;

public:
  vkBulletSystem();
  virtual ~vkBulletSystem();

  virtual void Initialize();

  virtual IPhysicsScene *CreateScene();

  virtual IPhysicsDynamicCollider *CreateDynamicCollider();
  virtual IPhysicsStaticCollider *CreateStaticCollider();
  virtual IPhysicsTriggerCollider *CreateTriggerCollider();
  virtual IPhysicsCapsuleCharacterController *CreateCapsulseCharacterController();

  virtual IPhysicsShape *CreateShape(const vkPhysGeometry &geometry);

  virtual IPhysicsJoint *CreateJoint(vkPhysicsJointType type, IPhysicsDynamicCollider *colliderA, IPhysicsDynamicCollider *colliderB);

};

