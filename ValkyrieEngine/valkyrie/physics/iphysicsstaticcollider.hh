#pragma once

#include <valkyrie/csexport.hh>
#include <valkyrie/physics/iphysicsbasecollider.hh>
#include <valkyrie/physics/iphysicsstaticcollider.refl.hh>

CS_INTERFACE()
struct CSE_API iPhysicsStaticCollider : public CS_SUPER(iPhysicsBaseCollider)
{
  CS_CLASS_GEN;
  virtual ~iPhysicsStaticCollider() { }

};