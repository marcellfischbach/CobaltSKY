#pragma once


#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/physics/iphysicsshape.hh>
#include <cobalt/physics/iphysicssystem.refl.hh>

namespace cs
{
struct iPhysicsScene;
struct iPhysicsCapsuleCharacterController;
struct iPhysicsDynamicCollider;
struct iPhysicsStaticCollider;
struct iPhysicsTriggerCollider;
struct iPhysicsJoint;

CS_CLASS()
struct CSE_API iPhysicsSystem : public cs::iObject
{
  CS_CLASS_GEN;
  virtual ~iPhysicsSystem() { }

  virtual void Initialize() = 0;

  virtual cs::iPhysicsScene* CreateScene() = 0;

  virtual cs::iPhysicsShape* CreateShape(const cs::PhysGeometry& geometry) = 0;

  virtual cs::iPhysicsDynamicCollider* CreateDynamicCollider() = 0;
  virtual cs::iPhysicsStaticCollider* CreateStaticCollider() = 0;
  virtual cs::iPhysicsTriggerCollider* CreateTriggerCollider() = 0;

  virtual cs::iPhysicsCapsuleCharacterController* CreateCapsulseCharacterController() = 0;


  virtual cs::iPhysicsJoint* CreateJoint(cs::ePhysicsJointType type, cs::iPhysicsDynamicCollider* colliderA, cs::iPhysicsDynamicCollider* colliderB) = 0;

};

}