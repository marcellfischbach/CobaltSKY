#pragma once

#include <physicsbullet/bulletexport.hh>
#include <physicsbullet/bulletjoint.hh>

#include <cobalt/physics/iphysicshingejoint.hh>
#include <physicsbullet/bullethingejoint.refl.hh>


class csBulletDynamicCollider;


CS_CLASS()
class CSBULLETPHYSICS_API csBulletHingeJoint : public  CS_SUPER(csBulletJoint), public CS_SUPER(cs::iPhysicsHingeJoint)
{
  CS_CLASS_GEN;
public:
  csBulletHingeJoint();
  virtual ~csBulletHingeJoint();

  bool Initialize(csBulletDynamicCollider *colliderA, csBulletDynamicCollider *colliderB);

  virtual void SetFrameA(const cs::Matrix4f &frame);
  virtual const cs::Matrix4f &GetFrameA() const;

  virtual void SetFrameB(const cs::Matrix4f &frame);
  virtual const cs::Matrix4f &GetFrameB() const;

  virtual void SetFrames(const cs::Matrix4f &frameA, const cs::Matrix4f &frameB);

  btHingeConstraint *GetHingeConstraint();
  const btHingeConstraint *GetHingeConstraint() const;

private:
  cs::Matrix4f m_frameA;
  cs::Matrix4f m_frameB;

  btHingeConstraint *m_hingeConstraints;
};



CS_FORCEINLINE btHingeConstraint *csBulletHingeJoint::GetHingeConstraint()
{
  return m_hingeConstraints;
}

CS_FORCEINLINE const btHingeConstraint *csBulletHingeJoint::GetHingeConstraint() const
{
  return m_hingeConstraints;
}


CS_FORCEINLINE const cs::Matrix4f &csBulletHingeJoint::GetFrameA() const
{
  return m_frameA;
}

CS_FORCEINLINE const cs::Matrix4f &csBulletHingeJoint::GetFrameB() const
{
  return m_frameB;
}
