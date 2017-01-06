#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/physics/iphysicsbasecollider.hh>
#include <valkyrie/physics/iphysicsstaticcollider.refl.hh>

VK_INTERFACE()
struct VKE_API IPhysicsStaticCollider : public VK_SUPER(IPhysicsBaseCollider)
{
  VK_CLASS_GEN;
  virtual ~IPhysicsStaticCollider() { }

};