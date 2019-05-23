
#include <physicsbullet/bullethingejoint.hh>
#include <physicsbullet/bulletscene.hh>

#include <btBulletDynamicsCommon.h>



csBulletHingeJoint::csBulletHingeJoint()
  : csBulletJoint(cs::ePJT_Hinge)
  , cs::iPhysicsHingeJoint()
{

}

csBulletHingeJoint::~csBulletHingeJoint()
{

}


bool csBulletHingeJoint::Initialize(csBulletDynamicCollider *colliderA, csBulletDynamicCollider *colliderB)
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
    m_hingeConstraints = new btHingeConstraint(*bodyA, *bodyB, trans, trans, true);
  }
  else if (bodyA)
  {
    m_hingeConstraints = new btHingeConstraint(*bodyA, trans, true);
  }
  else
  {
    m_hingeConstraints = new btHingeConstraint(*bodyB, trans, true);
  }

  SetConstraint(m_hingeConstraints);
  return m_hingeConstraints != 0;
}




void csBulletHingeJoint::SetFrameA(const cs::Matrix4f &frameA)
{
  m_frameA = frameA;
  btTransform transA, transB;
  transA.setFromOpenGLMatrix(static_cast<const btScalar*>(&m_frameA.m00));
  transB.setFromOpenGLMatrix(static_cast<const btScalar*>(&m_frameB.m00));

  m_hingeConstraints->setFrames(transA, transB);
}

void csBulletHingeJoint::SetFrameB(const cs::Matrix4f &frameB)
{
  m_frameB = frameB;

  btTransform transA, transB;
  transA.setFromOpenGLMatrix(static_cast<const btScalar*>(&m_frameA.m00));
  transB.setFromOpenGLMatrix(static_cast<const btScalar*>(&m_frameB.m00));

  m_hingeConstraints->setFrames(transA, transB);
}


void csBulletHingeJoint::SetFrames(const cs::Matrix4f &frameA, const cs::Matrix4f &frameB)
{
  m_frameA = frameA;
  m_frameB = frameB;

  btTransform transA, transB;
  transA.setFromOpenGLMatrix(static_cast<const btScalar*>(&m_frameA.m00));
  transB.setFromOpenGLMatrix(static_cast<const btScalar*>(&m_frameB.m00));

  m_hingeConstraints->setFrames(transA, transB);
}

