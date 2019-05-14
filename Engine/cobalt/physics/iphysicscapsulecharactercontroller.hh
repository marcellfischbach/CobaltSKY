#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/physics/iphysicscharactercontroller.hh>
#include <cobalt/physics/iphysicscapsulecharactercontroller.refl.hh>

CS_CLASS()
struct CSE_API iPhysicsCapsuleCharacterController : public CS_SUPER(iPhysicsCharacterController)
{
  CS_CLASS_GEN;
  virtual ~iPhysicsCapsuleCharacterController() { }

  virtual void Initialize(float height, float radius) = 0;
};

