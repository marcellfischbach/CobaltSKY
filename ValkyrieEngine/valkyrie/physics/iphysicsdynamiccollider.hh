#pragma once
#include <valkyrie/vkexport.hh>
#include <valkyrie/physics/iphysicsbasecollider.hh>
#include <valkyrie/physics/iphysicsdynamiccollider.refl.hh>

VK_INTERFACE()
struct VKE_API iPhysicsDynamicCollider : public VK_SUPER(iPhysicsBaseCollider)
{
  VK_CLASS_GEN;
  virtual ~iPhysicsDynamicCollider() { }

  virtual void SetTransformationCallback(iTransformationCallback *callback) = 0;

  virtual void SetKinematic(bool kinematic) = 0;
  virtual bool IsKinematic() const = 0;

  virtual void SetMass(float mass) = 0;
  virtual float GetMass() const = 0;

  virtual void SetInertia(const vkVector3f &inertia) = 0;
  virtual const vkVector3f &GetInertia() const = 0;

  virtual void SetAutoInertia(bool autoInertia) = 0;
  virtual bool IsAutoInertia() const = 0;

};
