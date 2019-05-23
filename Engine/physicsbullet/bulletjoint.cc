
#include <physicsbullet/bulletjoint.hh>
#include <physicsbullet/bulletscene.hh>

#include <btBulletDynamicsCommon.h>

cs::BulletJoint::BulletJoint(cs::ePhysicsJointType type)
  : m_type(type)
  , m_colliderA(0)
  , m_colliderB(0)
{

}

cs::BulletJoint::~BulletJoint()
{

}


cs::ePhysicsJointType cs::BulletJoint::GetType() const
{
  return m_type;
}

void cs::BulletJoint::SetConstraint(btTypedConstraint *constraint)
{
  m_constraint = constraint;
}

btTypedConstraint *cs::BulletJoint::GetConstraint()
{
  return m_constraint;
}


const btTypedConstraint *cs::BulletJoint::GetConstraint() const
{
  return m_constraint;
}

void cs::BulletJoint::SetCollider(cs::BulletDynamicCollider *colliderA, cs::BulletDynamicCollider *colliderB)
{
  if (m_colliderA)
  {
    m_colliderA->RemoveJoint(this);
  }
  if (m_colliderB)
  {
    m_colliderB->RemoveJoint(this);
  }
  CS_SET(m_colliderA, colliderA);
  CS_SET(m_colliderB, colliderB);
  if (m_colliderA)
  {
    m_colliderA->AddJoint(this);
  }
  if (m_colliderB)
  {
    m_colliderB->AddJoint(this);
  }
}

cs::iPhysicsDynamicCollider *cs::BulletJoint::GetColliderA() const
{
  return m_colliderA;
}

cs::iPhysicsDynamicCollider *cs::BulletJoint::GetColliderB() const
{
  return m_colliderB;
}

void cs::BulletJoint::AttachToScene(cs::BulletScene *scene)
{
  btDiscreteDynamicsWorld *world = scene->GetBulletScene();
  world->addConstraint(m_constraint, true);
}

void cs::BulletJoint::DetachFromScene(cs::BulletScene* scene)
{
  if (m_colliderA)
  {
    m_colliderA->RemoveJoint(this);
  }
  if (m_colliderB)
  {
    m_colliderB->RemoveJoint(this);
  }
  btDiscreteDynamicsWorld *world = scene->GetBulletScene();
  world->removeConstraint(m_constraint);
}
