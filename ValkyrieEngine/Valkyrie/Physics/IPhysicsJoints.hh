#pragma once

#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/vkdiamond.hh>
#include <Valkyrie/vkenums.hh>
#include <Valkyrie/Physics/IPhysicsCollider.hh>
#include <Valkyrie/Physics/IPhysicsJoints.refl.hh>


VK_INTERFACE()
struct VKE_API IPhysicsJoint : public IObject
{
  VK_CLASS_GEN;
  virtual ~IPhysicsJoint() { }

  virtual vkPhysicsJointType GetType() const = 0;

  virtual IPhysicsDynamicCollider *GetColliderA() const = 0;
  virtual IPhysicsDynamicCollider *GetColliderB() const = 0;

};


VK_INTERFACE()
struct VKE_API IPhysicsHingeJoint : public virtual VK_SUPER(IPhysicsJoint)
{
  VK_CLASS_GEN;
  virtual ~IPhysicsHingeJoint() { }


  virtual void SetFrameA(const vkMatrix4f &frame) = 0;
  virtual const vkMatrix4f &GetFrameA() const = 0;

  virtual void SetFrameB(const vkMatrix4f &frame) = 0;
  virtual const vkMatrix4f &GetFrameB() const = 0;

  virtual void SetFrames(const vkMatrix4f &frameA, const vkMatrix4f &frameB) = 0;
};


