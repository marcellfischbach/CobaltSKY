#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/physics/iphysicscollider.hh>
#include <Valkyrie/physics/iphysicsbasecollider.refl.hh>

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
