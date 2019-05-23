
#pragma once

#include <physicsbullet/bulletexport.hh>
#include <cobalt/physics/iphysicssystem.hh>
#include <physicsbullet/bulletsystem.refl.hh>



CS_CLASS()
class CSBULLETPHYSICS_API csBulletSystem : public CS_SUPER(cs::iPhysicsSystem)
{
  CS_CLASS_GEN_OBJECT;

public:
  csBulletSystem();
  virtual ~csBulletSystem();

  virtual void Initialize();

  virtual cs::iPhysicsScene *CreateScene();

  virtual cs::iPhysicsDynamicCollider *CreateDynamicCollider();
  virtual cs::iPhysicsStaticCollider *CreateStaticCollider();
  virtual cs::iPhysicsTriggerCollider *CreateTriggerCollider();
  virtual cs::iPhysicsCapsuleCharacterController *CreateCapsulseCharacterController();

  virtual cs::iPhysicsShape *CreateShape(const cs::PhysGeometry &geometry);

  virtual cs::iPhysicsJoint *CreateJoint(cs::ePhysicsJointType type, cs::iPhysicsDynamicCollider *colliderA, cs::iPhysicsDynamicCollider *colliderB);

};

