#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/Physics/iphysicsbasecollider.hh>
#include <Valkyrie/Physics/iphysicsstaticcollider.refl.hh>

VK_INTERFACE()
struct VKE_API IPhysicsStaticCollider : public VK_SUPER(IPhysicsBaseCollider)
{
  VK_CLASS_GEN;
  virtual ~IPhysicsStaticCollider() { }

};