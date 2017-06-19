#pragma once

#include <physicsbullet/bulletexport.hh>
#include <physicsbullet/BulletDynamicCollider.hh>

#include <valkyrie/physics/iphysicshingejoint.hh>
#include <physicsbullet/bulletjoint.refl.hh>


CS_INTERFACE()
class CSBULLETPHYSICS_API csBulletJoint : public virtual CS_SUPER(iPhysicsJoint)
{
  CS_CLASS_GEN_OBJECT;
public:
  virtual ~csBulletJoint();

  virtual csPhysicsJointType GetType() const;

  virtual iPhysicsDynamicCollider *GetColliderA() const;
  virtual iPhysicsDynamicCollider *GetColliderB() const;

  btTypedConstraint *GetConstraint();
  const btTypedConstraint *GetConstraint() const;

  virtual void AttachToScene(csBulletScene *scene);
  virtual void DetachFromScene(csBulletScene *scene);

protected:
  csBulletJoint(csPhysicsJointType type);

  void SetCollider(csBulletDynamicCollider *colliderA, csBulletDynamicCollider *colliderB);


  void SetConstraint(btTypedConstraint *constraint);

private:
  csPhysicsJointType m_type;

  csBulletDynamicCollider *m_colliderA;
  csBulletDynamicCollider *m_colliderB;

  btTypedConstraint *m_constraint;
};
