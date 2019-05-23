#pragma once

#include <csrefl/class.hh>
#include <cobalt/csdiamond.hh>
#include <cobalt/csenums.hh>
#include <cobalt/physics/iphysicscollider.hh>
#include <cobalt/physics/iphysicsjoint.refl.hh>

namespace cs
{

CS_CLASS()
struct CSE_API iPhysicsJoint : public cs::iObject
{
  CS_CLASS_GEN;
  virtual ~iPhysicsJoint() { }

  virtual cs::ePhysicsJointType GetType() const = 0;

  virtual cs::iPhysicsDynamicCollider* GetColliderA() const = 0;
  virtual cs::iPhysicsDynamicCollider* GetColliderB() const = 0;

};

}
