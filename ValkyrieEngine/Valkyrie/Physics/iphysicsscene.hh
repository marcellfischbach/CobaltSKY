#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/Physics/iphysicsscene.refl.hh>

struct IPhysicsCharacterController;
struct IPhysicsDynamicCollider;
struct IPhysicsJoint;
struct IPhysicsStaticCollider;

VK_INTERFACE()
struct VKE_API IPhysicsScene : public IObject
{
  VK_CLASS_GEN;
public:
  virtual ~IPhysicsScene () { }


  virtual void AddStaticCollider(IPhysicsStaticCollider *collider) = 0;
  virtual void RemoveStaticCollider(IPhysicsStaticCollider *collider) = 0;

  virtual void AddDynamicCollider(IPhysicsDynamicCollider *collider) = 0;
  virtual void RemoveDynamicCollider(IPhysicsDynamicCollider *collider) = 0;


  virtual void AddCharacterController(IPhysicsCharacterController *controller) = 0;
  virtual void RemoveCharacterController(IPhysicsCharacterController *controller) = 0;

  virtual void AddJoint(IPhysicsJoint *joint) = 0;
  virtual void RemoveJoint(IPhysicsJoint *joint) = 0;


  virtual void StepSimulation(float tpf = 1.0f / 60.0f) = 0;
  virtual void UpdateColliders() = 0;

};