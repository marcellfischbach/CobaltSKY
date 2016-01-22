#pragma once

#include <PhysicsBullet/Export.hh>
#include <Valkyrie/Physics/IPhysicsCharacterController.hh>

#include <PhysicsBullet/BulletCharacterController.refl.hh>

class btKinematicCharacterController;

VK_INTERFACE()
class VKBULLET_API vkBulletCharacterController : public IPhysicsCharacterController
{
  VK_CLASS_GEN_OBJECT;
public:
  virtual ~vkBulletCharacterController();

  virtual void WarpToPosition(const vkVector3f &newPosition);
  virtual void MoveLocal(const vkVector3f &direction);
  virtual void MoveGlobal(const vkVector3f &direction);
  virtual void Rotate(float angle);

  virtual bool IsOnGround() const;
  virtual bool CanJump() const;
  virtual void Jump();
  virtual void SetJumpSpeed(float jumpSpeed);

  virtual void SetMaxFallSpeed(float maxFallSpeed);
  virtual void SetGravity(const vkVector3f &gravity);

  virtual void SetMaxSlope(float maxSlope);

protected:
  vkBulletCharacterController();


  btKinematicCharacterController *m_characterController;

};