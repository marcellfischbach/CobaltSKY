#pragma once
#include <valkyrie/csexport.hh>
#include <valkyrie/physics/iphysicscollider.hh>
#include <valkyrie/physics/iphysicstriggercollider.refl.hh>

CS_INTERFACE()
struct CSE_API iPhysicsTriggerCollider : public CS_SUPER(iPhysicsCollider)
{
  CS_CLASS_GEN;
  virtual ~iPhysicsTriggerCollider() { }

};
