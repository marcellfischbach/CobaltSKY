#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/physics/iphysicscollider.hh>
#include <cobalt/physics/iphysicsbasecollider.refl.hh>

namespace cs
{

CS_CLASS()
struct CSE_API iPhysicsBaseCollider : public CS_SUPER(cs::iPhysicsCollider)
{
  CS_CLASS_GEN;
  virtual ~iPhysicsBaseCollider() { }

  virtual void SetFriction(float friction) = 0;
  virtual float GetFriction() const = 0;

  virtual void SetRestitution(float restitution) = 0;
  virtual float GetRestitution() const = 0;

};


}
