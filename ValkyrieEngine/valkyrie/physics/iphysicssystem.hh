#pragma once


#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/physics/iphysicsshape.hh>
#include <valkyrie/physics/iphysicssystem.refl.hh>

struct iPhysicsScene;
struct iPhysicsCapsuleCharacterController;
struct iPhysicsDynamicCollider;
struct iPhysicsStaticCollider;
struct iPhysicsTriggerCollider;
struct iPhysicsJoint;

VK_INTERFACE()
struct VKE_API iPhysicsSystem : public iObject
{
  VK_CLASS_GEN;
  virtual ~iPhysicsSystem () { }

  virtual void Initialize() = 0;

  virtual iPhysicsScene *CreateScene() = 0;

  virtual iPhysicsShape *CreateShape(const vkPhysGeometry &geometry) = 0;

  virtual iPhysicsDynamicCollider *CreateDynamicCollider() = 0;
  virtual iPhysicsStaticCollider *CreateStaticCollider() = 0;
  virtual iPhysicsTriggerCollider *CreateTriggerCollider() = 0;

  virtual iPhysicsCapsuleCharacterController *CreateCapsulseCharacterController() = 0;


  virtual iPhysicsJoint *CreateJoint(vkPhysicsJointType type, iPhysicsDynamicCollider *colliderA, iPhysicsDynamicCollider *colliderB) = 0;

};
