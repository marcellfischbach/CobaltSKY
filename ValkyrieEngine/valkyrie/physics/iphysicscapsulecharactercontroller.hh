#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/physics/iphysicscharactercontroller.hh>
#include <valkyrie/physics/iphysicscapsulecharactercontroller.refl.hh>

VK_INTERFACE()
struct VKE_API IPhysicsCapsuleCharacterController : public VK_SUPER(IPhysicsCharacterController)
{
  VK_CLASS_GEN;
  virtual ~IPhysicsCapsuleCharacterController() { }

  virtual void Initialize(float height, float radius) = 0;
};

