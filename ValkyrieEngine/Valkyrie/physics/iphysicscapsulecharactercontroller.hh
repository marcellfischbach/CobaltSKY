#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/physics/iphysicscharactercontroller.hh>
#include <Valkyrie/physics/iphysicscapsulecharactercontroller.refl.hh>

VK_INTERFACE()
struct VKE_API IPhysicsCapsuleCharacterController : public VK_SUPER(IPhysicsCharacterController)
{
  VK_CLASS_GEN;
  virtual ~IPhysicsCapsuleCharacterController() { }

  virtual void Initialize(float height, float radius) = 0;
};

