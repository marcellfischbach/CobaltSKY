#pragma once
#include <cobalt/csexport.hh>
#include <cobalt/physics/iphysicsbasecollider.hh>
#include <cobalt/physics/iphysicsdynamiccollider.refl.hh>


namespace cs
{

CS_CLASS()
struct CSE_API iPhysicsDynamicCollider : public CS_SUPER(cs::iPhysicsBaseCollider)
{
  CS_CLASS_GEN;
  virtual ~iPhysicsDynamicCollider() { }

  virtual void SetTransformationCallback(cs::iTransformationCallback * callback) = 0;

  virtual void SetKinematic(bool kinematic) = 0;
  virtual bool IsKinematic() const = 0;

  virtual void SetMass(float mass) = 0;
  virtual float GetMass() const = 0;

  virtual void SetInertia(const cs::Vector3f & inertia) = 0;
  virtual const cs::Vector3f& GetInertia() const = 0;

  virtual void SetAutoInertia(bool autoInertia) = 0;
  virtual bool IsAutoInertia() const = 0;

};

}