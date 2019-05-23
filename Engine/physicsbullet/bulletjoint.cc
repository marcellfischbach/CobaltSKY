
#include <physicsbullet/bulletjoint.hh>
#include <physicsbullet/bulletscene.hh>

#include <btBulletDynamicsCommon.h>

csBulletJoint::csBulletJoint(cs::ePhysicsJointType type)
  : m_type(type)
  , m_colliderA(0)
  , m_colliderB(0)
{

}

csBulletJoint::~csBulletJoint()
{

}


cs::ePhysicsJointType csBulletJoint::GetType() const
{
  return m_type;
}

void csBulletJoint::SetConstraint(btTypedConstraint *constraint)
{
  m_constraint = constraint;
}

btTypedConstraint *csBulletJoint::GetConstraint()
{
  return m_constraint;
}


const btTypedConstraint *csBulletJoint::GetConstraint() const
{
  return m_constraint;
}

void csBulletJoint::SetCollider(csBulletDynamicCollider *colliderA, csBulletDynamicCollider *colliderB)
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

cs::iPhysicsDynamicCollider *csBulletJoint::GetColliderA() const
{
  return m_colliderA;
}

cs::iPhysicsDynamicCollider *csBulletJoint::GetColliderB() const
{
  return m_colliderB;
}

void csBulletJoint::AttachToScene(csBulletScene *scene)
{
  btDiscreteDynamicsWorld *world = scene->GetBulletScene();
  world->addConstraint(m_constraint, true);
}

void csBulletJoint::DetachFromScene(csBulletScene* scene)
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
