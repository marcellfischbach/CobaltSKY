
#pragma once

#include <physicsbullet/bulletexport.hh>
#include <valkyrie/physics/iphysicssystem.hh>
#include <physicsbullet/bulletsystem.refl.hh>



CS_CLASS()
class CSBULLETPHYSICS_API csBulletSystem : public CS_SUPER(iPhysicsSystem)
{
  CS_CLASS_GEN_OBJECT;

public:
  csBulletSystem();
  virtual ~csBulletSystem();

  virtual void Initialize();

  virtual iPhysicsScene *CreateScene();

  virtual iPhysicsDynamicCollider *CreateDynamicCollider();
  virtual iPhysicsStaticCollider *CreateStaticCollider();
  virtual iPhysicsTriggerCollider *CreateTriggerCollider();
  virtual iPhysicsCapsuleCharacterController *CreateCapsulseCharacterController();

  virtual iPhysicsShape *CreateShape(const csPhysGeometry &geometry);

  virtual iPhysicsJoint *CreateJoint(csPhysicsJointType type, iPhysicsDynamicCollider *colliderA, iPhysicsDynamicCollider *colliderB);

};

