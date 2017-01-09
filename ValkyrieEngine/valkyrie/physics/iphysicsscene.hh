#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/physics/iphysicsscene.refl.hh>

struct iPhysicsCharacterController;
struct iPhysicsDynamicCollider;
struct iPhysicsJoint;
struct iPhysicsStaticCollider;

VK_INTERFACE()
struct VKE_API iPhysicsScene : public iObject
{
  VK_CLASS_GEN;
public:
  virtual ~iPhysicsScene () { }


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