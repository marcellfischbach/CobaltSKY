#pragma once

#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/vkdiamond.hh>
#include <Valkyrie/vkenums.hh>
#include <Valkyrie/physics/iphysicscollider.hh>
#include <Valkyrie/physics/iphysicsjoint.refl.hh>


VK_INTERFACE()
struct VKE_API IPhysicsJoint : public IObject
{
  VK_CLASS_GEN;
  virtual ~IPhysicsJoint() { }

  virtual vkPhysicsJointType GetType() const = 0;

  virtual IPhysicsDynamicCollider *GetColliderA() const = 0;
  virtual IPhysicsDynamicCollider *GetColliderB() const = 0;

};

