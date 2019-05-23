#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/physics/iphysicsbasecollider.hh>
#include <cobalt/physics/iphysicsstaticcollider.refl.hh>

namespace cs
{

CS_CLASS(NoInstance)
struct CSE_API iPhysicsStaticCollider : public CS_SUPER(cs::iPhysicsBaseCollider)
{
  CS_CLASS_GEN;
  virtual ~iPhysicsStaticCollider() { }

};

}