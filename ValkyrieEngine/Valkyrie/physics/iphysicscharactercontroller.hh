#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/math/vkmatrix.hh>
#include <Valkyrie/physics/iphysicscharactercontroller.refl.hh>

struct ITransformationCallback;

VK_INTERFACE()
struct VKE_API IPhysicsCharacterController : public IObject
{
  VK_CLASS_GEN;
  virtual ~IPhysicsCharacterController () { }

  virtual void SetTransformationCallback(ITransformationCallback *transformationCallback) = 0;

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

