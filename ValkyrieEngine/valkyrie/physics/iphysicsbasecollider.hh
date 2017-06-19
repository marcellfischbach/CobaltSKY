#pragma once

#include <valkyrie/csexport.hh>
#include <valkyrie/physics/iphysicscollider.hh>
#include <valkyrie/physics/iphysicsbasecollider.refl.hh>

CS_INTERFACE()
struct CSE_API iPhysicsBaseCollider : public CS_SUPER(iPhysicsCollider)
{
  CS_CLASS_GEN;
  virtual ~iPhysicsBaseCollider() { }

  virtual void SetFriction(float friction) = 0;
  virtual float GetFriction() const = 0;

  virtual void SetRestitution(float restitution) = 0;
  virtual float GetRestitution() const = 0;

};
