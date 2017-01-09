#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/math/vkmatrix.hh>
#include <valkyrie/physics/iphysicscharactercontroller.refl.hh>

struct iTransformationCallback;

VK_INTERFACE()
struct VKE_API iPhysicsCharacterController : public iObject
{
  VK_CLASS_GEN;
  virtual ~iPhysicsCharacterController () { }

  virtual void SetTransformationCallback(iTransformationCallback *transformationCallback) = 0;

  virtual void Warp(const vkMatrix4f &transformation) = 0;
  virtual void WarpToPosition(const vkVector3f &newPosition) = 0;
  virtual void SetLocalWalkDirection(const vkVector3f &direction) = 0;
  virtual void SetGlobalWalkDirection(const vkVector3f &direction) = 0;
  virtual void Rotate(float angle) = 0;

  virtual bool IsOnGround() const = 0;
  virtual bool CanJump() const = 0;
  virtual void Jump() = 0;
  virtual void SetJumpSpeed(float jumpSpeed) = 0;

  virtual void SetMaxFallSpeed(float maxFallSpeed) = 0;
  virtual void SetGravity(const vkVector3f &gravity) = 0;
  virtual void SetGravity(float gravity) = 0;

  virtual void SetMaxSlope(float maxSlope) = 0;  

};

