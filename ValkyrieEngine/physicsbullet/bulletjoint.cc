
#include <physicsbullet/bulletjoint.hh>
#include <physicsbullet/bulletscene.hh>

#include <btBulletDynamicsCommon.h>

vkBulletJoint::vkBulletJoint(vkPhysicsJointType type)
  : m_type(type)
  , m_colliderA(0)
  , m_colliderB(0)
{

}

vkBulletJoint::~vkBulletJoint()
{

}


vkPhysicsJointType vkBulletJoint::GetType() const
{
  return m_type;
}

void vkBulletJoint::SetConstraint(btTypedConstraint *constraint)
{
  m_constraint = constraint;
}

btTypedConstraint *vkBulletJoint::GetConstraint()
{
  return m_constraint;
}


const btTypedConstraint *vkBulletJoint::GetConstraint() const
{
  return m_constraint;
}

void vkBulletJoint::SetCollider(vkBulletDynamicCollider *colliderA, vkBulletDynamicCollider *colliderB)
{
  if (m_colliderA)
  {
    m_colliderA->RemoveJoint(this);
  }
  if (m_colliderB)
  {
    m_colliderB->RemoveJoint(this);
  }
  VK_SET(m_colliderA, colliderA);
  VK_SET(m_colliderB, colliderB);
  if (m_colliderA)
  {
    m_colliderA->AddJoint(this);
  }
  if (m_colliderB)
  {
    m_colliderB->AddJoint(this);
  }
}

IPhysicsDynamicCollider *vkBulletJoint::GetColliderA() const
{
  return m_colliderA;
}

IPhysicsDynamicCollider *vkBulletJoint::GetColliderB() const
{
  return m_colliderB;
}

void vkBulletJoint::AttachToScene(vkBulletScene *scene)
{
  btDiscreteDynamicsWorld *world = scene->GetBulletScene();
  world->addConstraint(m_constraint, true);
}

void vkBulletJoint::DetachFromScene(vkBulletScene* scene)
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
