#pragma once
#include <valkyrie/vkexport.hh>
#include <valkyrie/physics/iphysicscollider.hh>
#include <valkyrie/physics/iphysicstriggercollider.refl.hh>

VK_INTERFACE()
struct VKE_API iPhysicsTriggerCollider : public VK_SUPER(iPhysicsCollider)
{
  VK_CLASS_GEN;
  virtual ~iPhysicsTriggerCollider() { }

};
