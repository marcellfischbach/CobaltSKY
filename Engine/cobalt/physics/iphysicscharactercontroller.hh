#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/math/csmatrix.hh>
#include <cobalt/physics/iphysicscharactercontroller.refl.hh>

namespace cs
{

struct iTransformationCallback;


CS_CLASS()
struct CSE_API iPhysicsCharacterController : public cs::iObject
{
  CS_CLASS_GEN;
  virtual ~iPhysicsCharacterController () { }

  virtual void SetTransformationCallback(cs::iTransformationCallback *transformationCallback) = 0;

  virtual void Warp(const cs::Matrix4f &transformation) = 0;
  virtual void WarpToPosition(const cs::Vector3f &newPosition) = 0;
  virtual void SetLocalWalkDirection(const cs::Vector3f &direction) = 0;
  virtual void SetGlobalWalkDirection(const cs::Vector3f &direction) = 0;
  virtual void Rotate(float angle) = 0;

  virtual bool IsOnGround() const = 0;
  virtual bool CanJump() const = 0;
  virtual void Jump() = 0;
  virtual void SetJumpSpeed(float jumpSpeed) = 0;

  virtual void SetMaxFallSpeed(float maxFallSpeed) = 0;
  virtual void SetGravity(const cs::Vector3f &gravity) = 0;
  virtual void SetGravity(float gravity) = 0;

  virtual void SetMaxSlope(float maxSlope) = 0;  

};

}
