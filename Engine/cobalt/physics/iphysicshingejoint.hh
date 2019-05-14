#pragma once
#include <cobalt/csexport.hh>
#include <cobalt/physics/iphysicsjoint.hh>

#include <cobalt/physics/iphysicshingejoint.refl.hh>

CS_CLASS()
struct CSE_API iPhysicsHingeJoint : public virtual CS_SUPER(iPhysicsJoint)
{
  CS_CLASS_GEN;
  virtual ~iPhysicsHingeJoint() { }


  virtual void SetFrameA(const csMatrix4f &frame) = 0;
  virtual const csMatrix4f &GetFrameA() const = 0;

  virtual void SetFrameB(const csMatrix4f &frame) = 0;
  virtual const csMatrix4f &GetFrameB() const = 0;

  virtual void SetFrames(const csMatrix4f &frameA, const csMatrix4f &frameB) = 0;
};


