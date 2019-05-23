#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/physics/iphysicscharactercontroller.hh>
#include <cobalt/physics/iphysicscapsulecharactercontroller.refl.hh>

namespace cs
{


CS_CLASS()
struct CSE_API iPhysicsCapsuleCharacterController : public CS_SUPER(cs::iPhysicsCharacterController)
{
  CS_CLASS_GEN;
  virtual ~iPhysicsCapsuleCharacterController() { }

  virtual void Initialize(float height, float radius) = 0;
};


}

