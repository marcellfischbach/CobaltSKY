#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/physics/iphysicscharactercontroller.hh>
#include <valkyrie/physics/iphysicscapsulecharactercontroller.refl.hh>

VK_INTERFACE()
struct VKE_API iPhysicsCapsuleCharacterController : public VK_SUPER(iPhysicsCharacterController)
{
  VK_CLASS_GEN;
  virtual ~iPhysicsCapsuleCharacterController() { }

  virtual void Initialize(float height, float radius) = 0;
};

