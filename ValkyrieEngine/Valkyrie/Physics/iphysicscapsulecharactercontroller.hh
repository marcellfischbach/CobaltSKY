#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/Physics/iphysicscharactercontroller.hh>
#include <Valkyrie/Physics/iphysicscapsulecharactercontroller.refl.hh>

VK_INTERFACE()
struct VKE_API IPhysicsCapsuleCharacterController : public VK_SUPER(IPhysicsCharacterController)
{
  VK_CLASS_GEN;
  virtual ~IPhysicsCapsuleCharacterController() { }

  virtual void Initialize(float height, float radius) = 0;
};

