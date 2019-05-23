#pragma once

#include <physicsbullet/bulletexport.hh>
#include <physicsbullet/bulletjoint.hh>

#include <cobalt/physics/iphysicshingejoint.hh>
#include <physicsbullet/bullethingejoint.refl.hh>

namespace cs
{

class BulletDynamicCollider;


CS_CLASS()
class CSBULLETPHYSICS_API BulletHingeJoint : public  CS_SUPER(cs::BulletJoint), public CS_SUPER(cs::iPhysicsHingeJoint)
{
  CS_CLASS_GEN;
public:
  BulletHingeJoint();
  virtual ~BulletHingeJoint();

  bool Initialize(cs::BulletDynamicCollider * colliderA, cs::BulletDynamicCollider * colliderB);

  virtual void SetFrameA(const cs::Matrix4f & frame);
  virtual const cs::Matrix4f& GetFrameA() const;

  virtual void SetFrameB(const cs::Matrix4f & frame);
  virtual const cs::Matrix4f& GetFrameB() const;

  virtual void SetFrames(const cs::Matrix4f & frameA, const cs::Matrix4f & frameB);

  btHingeConstraint* GetHingeConstraint();
  const btHingeConstraint* GetHingeConstraint() const;

private:
  cs::Matrix4f m_frameA;
  cs::Matrix4f m_frameB;

  btHingeConstraint* m_hingeConstraints;
};

}


CS_FORCEINLINE btHingeConstraint *cs::BulletHingeJoint::GetHingeConstraint()
{
  return m_hingeConstraints;
}

CS_FORCEINLINE const btHingeConstraint *cs::BulletHingeJoint::GetHingeConstraint() const
{
  return m_hingeConstraints;
}


CS_FORCEINLINE const cs::Matrix4f &cs::BulletHingeJoint::GetFrameA() const
{
  return m_frameA;
}

CS_FORCEINLINE const cs::Matrix4f &cs::BulletHingeJoint::GetFrameB() const
{
  return m_frameB;
}

