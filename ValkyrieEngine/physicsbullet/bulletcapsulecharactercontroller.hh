#pragma once

#include <physicsbullet/bulletexport.hh>
#include <valkyrie/physics/iphysicscapsulecharactercontroller.hh>
#include <physicsbullet/bulletcapsulecharactercontroller.refl.hh>


class btCapsuleShape;
class btKinematicCharacterController;
class btPairCachingGhostObject;

class csBulletScene;

CS_CLASS()
class CSBULLETPHYSICS_API csBulletCapsuleCharacterController : public CS_SUPER(iPhysicsCapsuleCharacterController)
{
  CS_CLASS_GEN_OBJECT;
public:
  csBulletCapsuleCharacterController();
  virtual ~csBulletCapsuleCharacterController();

  virtual void Initialize(float height, float radius);


  virtual void SetTransformationCallback(iTransformationCallback *transformationCallback);

  virtual void Warp(const csMatrix4f &transformation);
  virtual void WarpToPosition(const csVector3f &newPosition);
  virtual void SetLocalWalkDirection(const csVector3f &direction);
  virtual void SetGlobalWalkDirection(const csVector3f &direction);
  virtual void Rotate(float angle);

  virtual bool IsOnGround() const;
  virtual bool CanJump() const;
  virtual void Jump();
  virtual void SetJumpSpeed(float jumpSpeed);

  virtual void SetMaxFallSpeed(float maxFallSpeed);
  virtual void SetGravity(const csVector3f &gravity);
  virtual void SetGravity(float gravity);

  virtual void SetMaxSlope(float maxSlope);

  void AttachToScene(csBulletScene *scene);
  void DetachFromScene(csBulletScene *scene);

  void UpdateCallbacks();
protected:
  iTransformationCallback *m_transformationCallback;
  btCapsuleShape *m_capsuleShape;
  btPairCachingGhostObject *m_ghostObject;
  btKinematicCharacterController *m_characterController;

};

