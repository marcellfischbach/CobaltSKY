#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/physics/iphysicsbasecollider.hh>
#include <cobalt/physics/iphysicsstaticcollider.refl.hh>

CS_INTERFACE()
struct CSE_API iPhysicsStaticCollider : public CS_SUPER(iPhysicsBaseCollider)
{
  CS_CLASS_GEN;
  virtual ~iPhysicsStaticCollider() { }

};