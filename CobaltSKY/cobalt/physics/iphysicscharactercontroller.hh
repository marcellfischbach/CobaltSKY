#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/math/csmatrix.hh>
#include <cobalt/physics/iphysicscharactercontroller.refl.hh>

struct iTransformationCallback;

CS_INTERFACE()
struct CSE_API iPhysicsCharacterController : public iObject
{
  CS_CLASS_GEN;
  virtual ~iPhysicsCharacterController () { }

  virtual void SetTransformationCallback(iTransformationCallback *transformationCallback) = 0;

  virtual void Warp(const csMatrix4f &transformation) = 0;
  virtual void WarpToPosition(const csVector3f &newPosition) = 0;
  virtual void SetLocalWalkDirection(const csVector3f &direction) = 0;
  virtual void SetGlobalWalkDirection(const csVector3f &direction) = 0;
  virtual void Rotate(float angle) = 0;

  virtual bool IsOnGround() const = 0;
  virtual bool CanJump() const = 0;
  virtual void Jump() = 0;
  virtual void SetJumpSpeed(float jumpSpeed) = 0;

  virtual void SetMaxFallSpeed(float maxFallSpeed) = 0;
  virtual void SetGravity(const csVector3f &gravity) = 0;
  virtual void SetGravity(float gravity) = 0;

  virtual void SetMaxSlope(float maxSlope) = 0;  

};

