#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/physics/iphysicsshape.hh>
#include <cobalt/physics/iphysicssystem.refl.hh>

struct iPhysicsScene;
struct iPhysicsCapsuleCharacterController;
struct iPhysicsDynamicCollider;
struct iPhysicsStaticCollider;
struct iPhysicsTriggerCollider;
struct iPhysicsJoint;

CS_INTERFACE()
struct CSE_API iPhysicsSystem : public iObject
{
  CS_CLASS_GEN;
  virtual ~iPhysicsSystem () { }

  virtual void Initialize() = 0;

  virtual iPhysicsScene *CreateScene() = 0;

  virtual iPhysicsShape *CreateShape(const csPhysGeometry &geometry) = 0;

  virtual iPhysicsDynamicCollider *CreateDynamicCollider() = 0;
  virtual iPhysicsStaticCollider *CreateStaticCollider() = 0;
  virtual iPhysicsTriggerCollider *CreateTriggerCollider() = 0;

  virtual iPhysicsCapsuleCharacterController *CreateCapsulseCharacterController() = 0;


  virtual iPhysicsJoint *CreateJoint(csPhysicsJointType type, iPhysicsDynamicCollider *colliderA, iPhysicsDynamicCollider *colliderB) = 0;

};
