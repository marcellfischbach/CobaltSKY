#pragma once
#include <cobalt/csexport.hh>
#include <cobalt/physics/iphysicsjoint.hh>

#include <cobalt/physics/iphysicshingejoint.refl.hh>

namespace cs
{

CS_CLASS()
struct CSE_API iPhysicsHingeJoint : public virtual CS_SUPER(cs::iPhysicsJoint)
{
  CS_CLASS_GEN;
  virtual ~iPhysicsHingeJoint() { }


  virtual void SetFrameA(const cs::Matrix4f & frame) = 0;
  virtual const cs::Matrix4f& GetFrameA() const = 0;

  virtual void SetFrameB(const cs::Matrix4f & frame) = 0;
  virtual const cs::Matrix4f& GetFrameB() const = 0;

  virtual void SetFrames(const cs::Matrix4f & frameA, const cs::Matrix4f & frameB) = 0;
};



}

