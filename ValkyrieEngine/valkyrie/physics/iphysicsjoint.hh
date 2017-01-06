#pragma once

#include <valkyrie/core/vkclass.hh>
#include <valkyrie/vkdiamond.hh>
#include <valkyrie/vkenums.hh>
#include <valkyrie/physics/iphysicscollider.hh>
#include <valkyrie/physics/iphysicsjoint.refl.hh>


VK_INTERFACE()
struct VKE_API IPhysicsJoint : public IObject
{
  VK_CLASS_GEN;
  virtual ~IPhysicsJoint() { }

  virtual vkPhysicsJointType GetType() const = 0;

  virtual IPhysicsDynamicCollider *GetColliderA() const = 0;
  virtual IPhysicsDynamicCollider *GetColliderB() const = 0;

};

