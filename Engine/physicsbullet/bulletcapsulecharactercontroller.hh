#pragma once

#include <physicsbullet/bulletexport.hh>
#include <cobalt/physics/iphysicscapsulecharactercontroller.hh>
#include <physicsbullet/bulletcapsulecharactercontroller.refl.hh>


class btCapsuleShape;
class btKinematicCharacterController;
class btPairCachingGhostObject;


namespace cs
{
class BulletScene;

CS_CLASS()
class CSBULLETPHYSICS_API BulletCapsuleCharacterController : public CS_SUPER(cs::iPhysicsCapsuleCharacterController)
{
  CS_CLASS_GEN_OBJECT;
public:
  BulletCapsuleCharacterController();
  virtual ~BulletCapsuleCharacterController();

  virtual void Initialize(float height, float radius);


  virtual void SetTransformationCallback(cs::iTransformationCallback * transformationCallback);

  virtual void Warp(const cs::Matrix4f & transformation);
  virtual void WarpToPosition(const cs::Vector3f & newPosition);
  virtual void SetLocalWalkDirection(const cs::Vector3f & direction);
  virtual void SetGlobalWalkDirection(const cs::Vector3f & direction);
  virtual void Rotate(float angle);

  virtual bool IsOnGround() const;
  virtual bool CanJump() const;
  virtual void Jump();
  virtual void SetJumpSpeed(float jumpSpeed);

  virtual void SetMaxFallSpeed(float maxFallSpeed);
  virtual void SetGravity(const cs::Vector3f & gravity);
  virtual void SetGravity(float gravity);

  virtual void SetMaxSlope(float maxSlope);

  void AttachToScene(cs::BulletScene * scene);
  void DetachFromScene(cs::BulletScene * scene);

  void UpdateCallbacks();
protected:
  cs::iTransformationCallback* m_transformationCallback;
  btCapsuleShape* m_capsuleShape;
  btPairCachingGhostObject* m_ghostObject;
  btKinematicCharacterController* m_characterController;

};


}

