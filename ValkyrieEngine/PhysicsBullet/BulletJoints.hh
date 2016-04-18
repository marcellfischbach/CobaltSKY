#pragma once

#include <PhysicsBullet/Export.hh>
#include <PhysicsBullet/BulletDynamicCollider.hh>

#include <Valkyrie/Physics/IPhysicsJoints.hh>


VK_INTERFACE()
class VKBULLET_API vkBulletJoint : public virtual IPhysicsJoint
{
  VK_CLASS_GEN_OBJECT;
public:
  virtual ~vkBulletJoint();

  virtual vkPhysicsJointType GetType() const;

  virtual IPhysicsDynamicCollider *GetColliderA() const;
  virtual IPhysicsDynamicCollider *GetColliderB() const;

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

VK_CLASS()
class VKBULLET_API vkBulletHingeJoint : public  vkBulletJoint, public  IPhysicsHingeJoint
{
  VK_CLASS_GEN;
public:
  vkBulletHingeJoint();
  virtual ~vkBulletHingeJoint();

  bool Initialize(vkBulletDynamicCollider *colliderA, vkBulletDynamicCollider *colliderB);

  virtual void SetFrameA(const vkMatrix4f &frame);
  virtual const vkMatrix4f &GetFrameA() const;

  virtual void SetFrameB(const vkMatrix4f &frame);
  virtual const vkMatrix4f &GetFrameB() const;

  virtual void SetFrames(const vkMatrix4f &frameA, const vkMatrix4f &frameB);

  btHingeConstraint *GetHingeConstraint();
  const btHingeConstraint *GetHingeConstraint() const;

private:
  vkMatrix4f m_frameA;
  vkMatrix4f m_frameB;

  btHingeConstraint *m_hingeConstraints;
};



VK_FORCEINLINE btHingeConstraint *vkBulletHingeJoint::GetHingeConstraint()
{
  return m_hingeConstraints;
}

VK_FORCEINLINE const btHingeConstraint *vkBulletHingeJoint::GetHingeConstraint() const
{
  return m_hingeConstraints;
}


VK_FORCEINLINE const vkMatrix4f &vkBulletHingeJoint::GetFrameA() const
{
  return m_frameA;
}

VK_FORCEINLINE const vkMatrix4f &vkBulletHingeJoint::GetFrameB() const
{
  return m_frameB;
}
