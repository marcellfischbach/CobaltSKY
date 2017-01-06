
#pragma once

#include <physicsbullet/bulletexport.hh>
#include <valkyrie/physics/iphysicssystem.hh>
#include <physicsbullet/bulletsystem.refl.hh>



VK_CLASS()
class VKBULLETPHYSICS_API vkBulletSystem : public VK_SUPER(IPhysicsSystem)
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

