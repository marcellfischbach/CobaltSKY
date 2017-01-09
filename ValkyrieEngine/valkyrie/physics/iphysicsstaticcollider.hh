#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/physics/iphysicsbasecollider.hh>
#include <valkyrie/physics/iphysicsstaticcollider.refl.hh>

VK_INTERFACE()
struct VKE_API iPhysicsStaticCollider : public VK_SUPER(iPhysicsBaseCollider)
{
  VK_CLASS_GEN;
  virtual ~iPhysicsStaticCollider() { }

};