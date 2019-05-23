#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/physics/iphysicsscene.refl.hh>

namespace cs
{

struct iPhysicsCharacterController;
struct iPhysicsCollider;
struct iPhysicsDynamicCollider;
struct iPhysicsJoint;
struct iPhysicsStaticCollider;

CS_CLASS()
struct CSE_API iPhysicsScene : public cs::iObject
{
  CS_CLASS_GEN;
public:
  virtual ~iPhysicsScene() { }

  virtual void AddCollider(cs::iPhysicsCollider* collider) = 0;
  virtual void RemoveCollider(cs::iPhysicsCollider* collider) = 0;

  virtual void AddStaticCollider(cs::iPhysicsStaticCollider* collider) = 0;
  virtual void RemoveStaticCollider(cs::iPhysicsStaticCollider* collider) = 0;

  virtual void AddDynamicCollider(cs::iPhysicsDynamicCollider* collider) = 0;
  virtual void RemoveDynamicCollider(cs::iPhysicsDynamicCollider* collider) = 0;


  virtual void AddCharacterController(cs::iPhysicsCharacterController* controller) = 0;
  virtual void RemoveCharacterController(cs::iPhysicsCharacterController* controller) = 0;

  virtual void AddJoint(cs::iPhysicsJoint* joint) = 0;
  virtual void RemoveJoint(cs::iPhysicsJoint* joint) = 0;


  virtual void StepSimulation(float tpf = 1.0f / 60.0f) = 0;
  virtual void UpdateColliders() = 0;

};

}