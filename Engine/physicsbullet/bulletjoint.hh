#pragma once

#include <physicsbullet/bulletexport.hh>
#include <physicsbullet/BulletDynamicCollider.hh>

#include <cobalt/physics/iphysicshingejoint.hh>
#include <physicsbullet/bulletjoint.refl.hh>


CS_CLASS()
class CSBULLETPHYSICS_API csBulletJoint : public virtual CS_SUPER(cs::iPhysicsJoint)
{
  CS_CLASS_GEN_OBJECT;
public:
  virtual ~csBulletJoint();

  virtual cs::ePhysicsJointType GetType() const;

  virtual cs::iPhysicsDynamicCollider *GetColliderA() const;
  virtual cs::iPhysicsDynamicCollider *GetColliderB() const;

  btTypedConstraint *GetConstraint();
  const btTypedConstraint *GetConstraint() const;

  virtual void AttachToScene(csBulletScene *scene);
  virtual void DetachFromScene(csBulletScene *scene);

protected:
  csBulletJoint(cs::ePhysicsJointType type);

  void SetCollider(csBulletDynamicCollider *colliderA, csBulletDynamicCollider *colliderB);


  void SetConstraint(btTypedConstraint *constraint);

private:
  cs::ePhysicsJointType m_type;

  csBulletDynamicCollider *m_colliderA;
  csBulletDynamicCollider *m_colliderB;

  btTypedConstraint *m_constraint;
};
