#pragma once

#include <physicsbullet/bulletexport.hh>
#include <physicsbullet/BulletDynamicCollider.hh>

#include <valkyrie/physics/iphysicshingejoint.hh>
#include <physicsbullet/bulletjoint.refl.hh>


VK_INTERFACE()
class VKBULLETPHYSICS_API vkBulletJoint : public virtual VK_SUPER(iPhysicsJoint)
{
  VK_CLASS_GEN_OBJECT;
public:
  virtual ~vkBulletJoint();

  virtual vkPhysicsJointType GetType() const;

  virtual iPhysicsDynamicCollider *GetColliderA() const;
  virtual iPhysicsDynamicCollider *GetColliderB() const;

  btTypedConstraint *GetConstraint();
  const btTypedConstraint *GetConstraint() const;

  virtual void AttachToScene(vkBulletScene *scene);
  virtual void DetachFromScene(vkBulletScene *scene);

protected:
  vkBulletJoint(vkPhysicsJointType type);

  void SetCollider(vkBulletDynamicCollider *colliderA, vkBulletDynamicCollider *colliderB);


  void SetConstraint(btTypedConstraint *constraint);

private:
  vkPhysicsJointType m_type;

  vkBulletDynamicCollider *m_colliderA;
  vkBulletDynamicCollider *m_colliderB;

  btTypedConstraint *m_constraint;
};
