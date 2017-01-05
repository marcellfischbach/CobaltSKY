#pragma once
#include <Valkyrie/vkexport.hh>
#include <Valkyrie/Physics/iphysicsbasecollider.hh>
#include <Valkyrie/Physics/iphysicsdynamiccollider.refl.hh>

VK_INTERFACE()
struct VKE_API IPhysicsDynamicCollider : public VK_SUPER(IPhysicsBaseCollider)
{
  VK_CLASS_GEN;
  virtual ~IPhysicsDynamicCollider() { }

  virtual void SetTransformationCallback(ITransformationCallback *callback) = 0;

  virtual void SetKinematic(bool kinematic) = 0;
  virtual bool IsKinematic() const = 0;

  virtual void SetMass(float mass) = 0;
  virtual float GetMass() const = 0;

  virtual void SetInertia(const vkVector3f &inertia) = 0;
  virtual const vkVector3f &GetInertia() const = 0;

  virtual void SetAutoInertia(bool autoInertia) = 0;
  virtual bool IsAutoInertia() const = 0;

};
