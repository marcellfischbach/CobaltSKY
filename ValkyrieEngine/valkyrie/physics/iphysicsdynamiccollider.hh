#pragma once
#include <valkyrie/csexport.hh>
#include <valkyrie/physics/iphysicsbasecollider.hh>
#include <valkyrie/physics/iphysicsdynamiccollider.refl.hh>

CS_INTERFACE()
struct CSE_API iPhysicsDynamicCollider : public CS_SUPER(iPhysicsBaseCollider)
{
  CS_CLASS_GEN;
  virtual ~iPhysicsDynamicCollider() { }

  virtual void SetTransformationCallback(iTransformationCallback *callback) = 0;

  virtual void SetKinematic(bool kinematic) = 0;
  virtual bool IsKinematic() const = 0;

  virtual void SetMass(float mass) = 0;
  virtual float GetMass() const = 0;

  virtual void SetInertia(const csVector3f &inertia) = 0;
  virtual const csVector3f &GetInertia() const = 0;

  virtual void SetAutoInertia(bool autoInertia) = 0;
  virtual bool IsAutoInertia() const = 0;

};
