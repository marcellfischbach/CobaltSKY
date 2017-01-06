#pragma once


#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/physics/iphysicsshape.hh>
#include <valkyrie/physics/iphysicssystem.refl.hh>

struct IPhysicsScene;
struct IPhysicsCapsuleCharacterController;
struct IPhysicsDynamicCollider;
struct IPhysicsStaticCollider;
struct IPhysicsTriggerCollider;
struct IPhysicsJoint;

VK_INTERFACE()
struct VKE_API IPhysicsSystem : public IObject
{
  VK_CLASS_GEN;
  virtual ~IPhysicsSystem () { }

  virtual void Initialize() = 0;

  virtual IPhysicsScene *CreateScene() = 0;

  virtual IPhysicsShape *CreateShape(const vkPhysGeometry &geometry) = 0;

  virtual IPhysicsDynamicCollider *CreateDynamicCollider() = 0;
  virtual IPhysicsStaticCollider *CreateStaticCollider() = 0;
  virtual IPhysicsTriggerCollider *CreateTriggerCollider() = 0;

  virtual IPhysicsCapsuleCharacterController *CreateCapsulseCharacterController() = 0;


  virtual IPhysicsJoint *CreateJoint(vkPhysicsJointType type, IPhysicsDynamicCollider *colliderA, IPhysicsDynamicCollider *colliderB) = 0;

};