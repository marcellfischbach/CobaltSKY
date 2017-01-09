
#pragma once

#include <physicsbullet/bulletexport.hh>
#include <valkyrie/physics/iphysicssystem.hh>
#include <physicsbullet/bulletsystem.refl.hh>



VK_CLASS()
class VKBULLETPHYSICS_API vkBulletSystem : public VK_SUPER(iPhysicsSystem)
{
  VK_CLASS_GEN_OBJECT;

public:
  vkBulletSystem();
  virtual ~vkBulletSystem();

  virtual void Initialize();

  virtual iPhysicsScene *CreateScene();

  virtual iPhysicsDynamicCollider *CreateDynamicCollider();
  virtual iPhysicsStaticCollider *CreateStaticCollider();
  virtual iPhysicsTriggerCollider *CreateTriggerCollider();
  virtual iPhysicsCapsuleCharacterController *CreateCapsulseCharacterController();

  virtual iPhysicsShape *CreateShape(const vkPhysGeometry &geometry);

  virtual iPhysicsJoint *CreateJoint(vkPhysicsJointType type, iPhysicsDynamicCollider *colliderA, iPhysicsDynamicCollider *colliderB);

};

