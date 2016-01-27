
#include <PhysicsBullet/BulletJoints.hh>
#include <PhysicsBullet/BulletScene.hh>

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
  VK_SET(m_colliderA, colliderA);
  VK_SET(m_colliderB, colliderB);
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
  world->addConstraint(m_constraint);

}

void vkBulletJoint::DetachFromScene(vkBulletScene* scene)
{
  btDiscreteDynamicsWorld *world = scene->GetBulletScene();
  world->removeConstraint(m_constraint);
}


vkBulletHingeJoint::vkBulletHingeJoint()
  : vkBulletJoint(ePJT_Hinge)
  , IPhysicsHingeJoint()
{

}

vkBulletHingeJoint::~vkBulletHingeJoint()
{

}


bool vkBulletHingeJoint::Initialize(vkBulletDynamicCollider *colliderA, vkBulletDynamicCollider *colliderB)
{
  SetCollider(colliderA, colliderB);
  btRigidBody *bodyA = colliderA ? colliderA->GetRigidBody() : 0;
  btRigidBody *bodyB = colliderB ? colliderB->GetRigidBody() : 0;

  if (!bodyA && !bodyB)
  {
    return false;
  }

  btTransform trans;
  trans.setIdentity();
  if (bodyA && bodyB)
  {
    m_hingeConstraints = new btHingeConstraint(*bodyA, *bodyB, trans, trans, false);
  }
  else if (bodyA)
  {
    m_hingeConstraints = new btHingeConstraint(*bodyA, trans, false);
  }
  else 
  {
    m_hingeConstraints = new btHingeConstraint(*bodyB, trans, false);
  }

  return m_hingeConstraints != 0;
}




void vkBulletHingeJoint::SetFrameA(const vkMatrix4f &frameA)
{
  m_frameA = frameA;
  btTransform transA, transB;
  transA.setFromOpenGLMatrix(static_cast<const btScalar*>(&m_frameA.m00));
  transB.setFromOpenGLMatrix(static_cast<const btScalar*>(&m_frameB.m00));

  m_hingeConstraints->setFrames(transA, transB);
}

void vkBulletHingeJoint::SetFrameB(const vkMatrix4f &frameB)
{
  m_frameB = frameB;

  btTransform transA, transB;
  transA.setFromOpenGLMatrix(static_cast<const btScalar*>(&m_frameA.m00));
  transB.setFromOpenGLMatrix(static_cast<const btScalar*>(&m_frameB.m00));

  m_hingeConstraints->setFrames(transA, transB);
}


void vkBulletHingeJoint::SetFrames(const vkMatrix4f &frameA, const vkMatrix4f &frameB)
{
  m_frameA = frameA;
  m_frameB = frameB;

  btTransform transA, transB;
  transA.setFromOpenGLMatrix(static_cast<const btScalar*>(&m_frameA.m00));
  transB.setFromOpenGLMatrix(static_cast<const btScalar*>(&m_frameB.m00));

  m_hingeConstraints->setFrames(transA, transB);
}

