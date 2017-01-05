#pragma once
#include <Valkyrie/vkexport.hh>
#include <Valkyrie/Physics/iphysicsjoint.hh>

#include <Valkyrie/Physics/iphysicshingejoint.refl.hh>

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


