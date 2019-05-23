#pragma once
#include <cobalt/csexport.hh>
#include <cobalt/physics/iphysicscollider.hh>
#include <cobalt/physics/iphysicstriggercollider.refl.hh>

namespace cs
{

CS_CLASS(NoInstance)
struct CSE_API iPhysicsTriggerCollider : public CS_SUPER(cs::iPhysicsCollider)
{
  CS_CLASS_GEN;
  virtual ~iPhysicsTriggerCollider() { }

};

}