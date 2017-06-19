#pragma once
#include <valkyrie/csexport.hh>
#include <valkyrie/physics/iphysicsjoint.hh>

#include <valkyrie/physics/iphysicshingejoint.refl.hh>

CS_INTERFACE()
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


