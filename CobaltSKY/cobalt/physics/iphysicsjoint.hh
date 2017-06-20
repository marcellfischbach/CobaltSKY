#pragma once

#include <cobalt/core/csclass.hh>
#include <cobalt/csdiamond.hh>
#include <cobalt/csenums.hh>
#include <cobalt/physics/iphysicscollider.hh>
#include <cobalt/physics/iphysicsjoint.refl.hh>


CS_INTERFACE()
struct CSE_API iPhysicsJoint : public iObject
{
  CS_CLASS_GEN;
  virtual ~iPhysicsJoint() { }

  virtual csPhysicsJointType GetType() const = 0;

  virtual iPhysicsDynamicCollider *GetColliderA() const = 0;
  virtual iPhysicsDynamicCollider *GetColliderB() const = 0;

};

