#pragma once
#include <valkyrie/vkexport.hh>
#include <valkyrie/physics/iphysicscollider.hh>
#include <valkyrie/physics/iphysicstriggercollider.refl.hh>

VK_INTERFACE()
struct VKE_API IPhysicsTriggerCollider : public VK_SUPER(IPhysicsCollider)
{
  VK_CLASS_GEN;
  virtual ~IPhysicsTriggerCollider() { }

};
