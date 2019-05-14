#pragma once
#include <cobalt/csexport.hh>
#include <cobalt/physics/iphysicscollider.hh>
#include <cobalt/physics/iphysicstriggercollider.refl.hh>

CS_CLASS(NoInstance)
struct CSE_API iPhysicsTriggerCollider : public CS_SUPER(iPhysicsCollider)
{
  CS_CLASS_GEN;
  virtual ~iPhysicsTriggerCollider() { }

};
