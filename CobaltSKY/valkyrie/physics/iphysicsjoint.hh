#pragma once

#include <valkyrie/core/csclass.hh>
#include <valkyrie/csdiamond.hh>
#include <valkyrie/csenums.hh>
#include <valkyrie/physics/iphysicscollider.hh>
#include <valkyrie/physics/iphysicsjoint.refl.hh>


CS_INTERFACE()
struct CSE_API iPhysicsJoint : public iObject
{
  CS_CLASS_GEN;
  virtual ~iPhysicsJoint() { }

  virtual csPhysicsJointType GetType() const = 0;

  virtual iPhysicsDynamicCollider *GetColliderA() const = 0;
  virtual iPhysicsDynamicCollider *GetColliderB() const = 0;

};

