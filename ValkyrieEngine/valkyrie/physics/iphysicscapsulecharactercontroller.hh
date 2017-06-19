#pragma once

#include <valkyrie/csexport.hh>
#include <valkyrie/physics/iphysicscharactercontroller.hh>
#include <valkyrie/physics/iphysicscapsulecharactercontroller.refl.hh>

CS_INTERFACE()
struct CSE_API iPhysicsCapsuleCharacterController : public CS_SUPER(iPhysicsCharacterController)
{
  CS_CLASS_GEN;
  virtual ~iPhysicsCapsuleCharacterController() { }

  virtual void Initialize(float height, float radius) = 0;
};

