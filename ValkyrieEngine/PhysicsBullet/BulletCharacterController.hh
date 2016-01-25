#pragma once

#include <PhysicsBullet/Export.hh>
#include <Valkyrie/Physics/IPhysicsCharacterController.hh>

#include <PhysicsBullet/BulletCharacterController.refl.hh>

class btCapsuleShape;
class btKinematicCharacterController;
class btPairCachingGhostObject;

class vkBulletScene;

VK_CLASS()
class VKBULLET_API vkBulletCapsuleCharacterController : public IPhysicsCapsuleCharacterController
{
  VK_CLASS_GEN_OBJECT;
public:
  vkBulletCapsuleCharacterController();
  virtual ~vkBulletCapsuleCharacterController();

  virtual void Initialize(float height, float radius);


  virtual void SetTransformationCallback(ITransformationCallback *transformationCallback);

  virtual void Warp(const vkMatrix4f &transformation);
  virtual void WarpToPosition(const vkVector3f &newPosition);
  virtual void SetLocalWalkDirection(const vkVector3f &direction);
  virtual void SetGlobalWalkDirection(const vkVector3f &direction);
  virtual void Rotate(float angle);

  virtual bool IsOnGround() const;
  virtual bool CanJump() const;
  virtual void Jump();
  virtual void SetJumpSpeed(float jumpSpeed);

  virtual void SetMaxFallSpeed(float maxFallSpeed);
  virtual void SetGravity(const vkVector3f &gravity);
  virtual void SetGravity(float gravity);

  virtual void SetMaxSlope(float maxSlope);

  void AttachToScene(vkBulletScene *scene);
  void DetachFromScene(vkBulletScene *scene);

  void UpdateCallbacks();
protected:
  ITransformationCallback *m_transformationCallback;
  btCapsuleShape *m_capsuleShape;
  btPairCachingGhostObject *m_ghostObject;
  btKinematicCharacterController *m_characterController;

};

