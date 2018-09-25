#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/physics/iphysicsscene.refl.hh>

struct iPhysicsCharacterController;
struct iPhysicsCollider;
struct iPhysicsDynamicCollider;
struct iPhysicsJoint;
struct iPhysicsStaticCollider;

CS_INTERFACE()
struct CSE_API iPhysicsScene : public iObject
{
  CS_CLASS_GEN;
public:
  virtual ~iPhysicsScene () { }

  virtual void AddCollider(iPhysicsCollider *collider) = 0;
  virtual void RemoveCollider(iPhysicsCollider *collider) = 0;

  virtual void AddStaticCollider(iPhysicsStaticCollider *collider) = 0;
  virtual void RemoveStaticCollider(iPhysicsStaticCollider *collider) = 0;

  virtual void AddDynamicCollider(iPhysicsDynamicCollider *collider) = 0;
  virtual void RemoveDynamicCollider(iPhysicsDynamicCollider *collider) = 0;


  virtual void AddCharacterController(iPhysicsCharacterController *controller) = 0;
  virtual void RemoveCharacterController(iPhysicsCharacterController *controller) = 0;

  virtual void AddJoint(iPhysicsJoint *joint) = 0;
  virtual void RemoveJoint(iPhysicsJoint *joint) = 0;


  virtual void StepSimulation(float tpf = 1.0f / 60.0f) = 0;
  virtual void UpdateColliders() = 0;

};
