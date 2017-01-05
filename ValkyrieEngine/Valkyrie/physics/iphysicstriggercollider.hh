#pragma once
#include <Valkyrie/vkexport.hh>
#include <Valkyrie/physics/iphysicscollider.hh>
#include <Valkyrie/physics/iphysicstriggercollider.refl.hh>

VK_INTERFACE()
struct VKE_API IPhysicsTriggerCollider : public VK_SUPER(IPhysicsCollider)
{
  VK_CLASS_GEN;
  virtual ~IPhysicsTriggerCollider() { }

};
