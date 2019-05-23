#pragma once

#include <physicsbullet/bulletexport.hh>
#include <physicsbullet/BulletDynamicCollider.hh>

#include <cobalt/physics/iphysicshingejoint.hh>
#include <physicsbullet/bulletjoint.refl.hh>

namespace cs
{

CS_CLASS()
class CSBULLETPHYSICS_API BulletJoint : public virtual CS_SUPER(cs::iPhysicsJoint)
{
  CS_CLASS_GEN_OBJECT;
public:
  virtual ~BulletJoint();

  virtual cs::ePhysicsJointType GetType() const;

  virtual cs::iPhysicsDynamicCollider* GetColliderA() const;
  virtual cs::iPhysicsDynamicCollider* GetColliderB() const;

  btTypedConstraint* GetConstraint();
  const btTypedConstraint* GetConstraint() const;

  virtual void AttachToScene(cs::BulletScene * scene);
  virtual void DetachFromScene(cs::BulletScene * scene);

protected:
  BulletJoint(cs::ePhysicsJointType type);

  void SetCollider(cs::BulletDynamicCollider * colliderA, cs::BulletDynamicCollider * colliderB);


  void SetConstraint(btTypedConstraint * constraint);

private:
  cs::ePhysicsJointType m_type;

  cs::BulletDynamicCollider* m_colliderA;
  cs::BulletDynamicCollider* m_colliderB;

  btTypedConstraint* m_constraint;
};

}

