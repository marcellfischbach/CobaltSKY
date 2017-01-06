#pragma once

#include <physicsbullet/bulletexport.hh>
#include <physicsbullet/bulletjoint.hh>

#include <valkyrie/physics/iphysicshingejoint.hh>
#include <physicsbullet/bullethingejoint.refl.hh>


class vkBulletDynamicCollider;


VK_CLASS()
class VKBULLETPHYSICS_API vkBulletHingeJoint : public  VK_SUPER(vkBulletJoint), public VK_SUPER(IPhysicsHingeJoint)
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
