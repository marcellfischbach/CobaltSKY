#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/physics/iphysicscollider.hh>
#include <valkyrie/physics/iphysicsbasecollider.refl.hh>

VK_INTERFACE()
struct VKE_API IPhysicsBaseCollider : public VK_SUPER(IPhysicsCollider)
{
  VK_CLASS_GEN;
  virtual ~IPhysicsBaseCollider() { }

  virtual void SetFriction(float friction) = 0;
  virtual float GetFriction() const = 0;

  virtual void SetRestitution(float restitution) = 0;
  virtual float GetRestitution() const = 0;

};
