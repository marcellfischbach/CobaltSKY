#pragma once
#include <valkyrie/vkexport.hh>
#include <valkyrie/physics/iphysicsjoint.hh>

#include <valkyrie/physics/iphysicshingejoint.refl.hh>

VK_INTERFACE()
struct VKE_API iPhysicsHingeJoint : public virtual VK_SUPER(iPhysicsJoint)
{
  VK_CLASS_GEN;
  virtual ~iPhysicsHingeJoint() { }


  virtual void SetFrameA(const vkMatrix4f &frame) = 0;
  virtual const vkMatrix4f &GetFrameA() const = 0;

  virtual void SetFrameB(const vkMatrix4f &frame) = 0;
  virtual const vkMatrix4f &GetFrameB() const = 0;

  virtual void SetFrames(const vkMatrix4f &frameA, const vkMatrix4f &frameB) = 0;
};


