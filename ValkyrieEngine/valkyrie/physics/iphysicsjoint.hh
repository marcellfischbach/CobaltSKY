#pragma once

#include <valkyrie/core/vkclass.hh>
#include <valkyrie/vkdiamond.hh>
#include <valkyrie/vkenums.hh>
#include <valkyrie/physics/iphysicscollider.hh>
#include <valkyrie/physics/iphysicsjoint.refl.hh>


VK_INTERFACE()
struct VKE_API iPhysicsJoint : public iObject
{
  VK_CLASS_GEN;
  virtual ~iPhysicsJoint() { }

  virtual vkPhysicsJointType GetType() const = 0;

  virtual iPhysicsDynamicCollider *GetColliderA() const = 0;
  virtual iPhysicsDynamicCollider *GetColliderB() const = 0;

};

