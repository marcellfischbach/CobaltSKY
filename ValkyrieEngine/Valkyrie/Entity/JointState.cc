
#include <Valkyrie/Entity/JointState.hh>
#include <Valkyrie/Entity/ColliderState.hh>
#include <Valkyrie/Physics/IPhysicsJoints.hh>
#include <Valkyrie/Physics/IPhysicsSystem.hh>
#include <Valkyrie/Engine.hh>



vkJointState::vkJointState()
  : vkSpatialState()
  , m_colliderA(0)
  , m_colliderB(0)
  , m_transformReference(eTR_ColliderA)
{

}

vkJointState::~vkJointState()
{
  VK_RELEASE(m_colliderA);
  VK_RELEASE(m_colliderB);
}

void vkJointState::SetColliderA(vkDynamicColliderState *collider)
{
  VK_SET(m_colliderA, collider);
}

void vkJointState::SetColliderB(vkDynamicColliderState *collider)
{
  VK_SET(m_colliderB, collider);
}



vkHingeJointState::vkHingeJointState()
  : vkJointState()
  , m_hingeJoint(0)
{

}

vkHingeJointState::~vkHingeJointState()
{

}


void vkHingeJointState::UpdateTransformation()
{
  vkSpatialState::UpdateTransformation();

  if (!m_hingeJoint)
  {
    ConnectState();
    if (!m_hingeJoint)
    {
      return;
    }
  }

  vkDynamicColliderState *colA = GetColliderA();
  vkDynamicColliderState *colB = GetColliderB();
  if (colA && colB)
  {
    return;
  }

  const vkMatrix4f &globalMatrix = GetLocalTransformation();

  vkMatrix4f frameA, frameB;
  if (colA)
  {
    vkMatrix4f calInv = colA->GetGlobalTransformationInv();
    vkMatrix4f::Mult(calInv, globalMatrix, frameA);
    m_hingeJoint->SetFrameA(frameA);
  }
  
  if (colB)
  {
    vkMatrix4f calInv = colB->GetGlobalTransformationInv();
    vkMatrix4f::Mult(calInv, globalMatrix, frameB);
    m_hingeJoint->SetFrameA(frameB);
  }

}

void vkHingeJointState::ConnectState()
{
  if (m_hingeJoint)
  {
    return;
  }

  vkDynamicColliderState *colA = GetColliderA();
  vkDynamicColliderState *colB = GetColliderB();
  IPhysicsDynamicCollider *colliderA = colA ? colA->GetDynamicCollider() : 0;
  IPhysicsDynamicCollider *colliderB = colB ? colB->GetDynamicCollider() : 0;
  IPhysicsJoint *joint = vkEngine::Get()->GetPhysicsSystem()->CreateJoint(ePJT_Hinge, colliderA, colliderB);
  printf("Joint: %p\n", joint);
  m_hingeJoint = vkQueryClass<IPhysicsHingeJoint>(joint);
  if (!m_hingeJoint)
  {
    printf("no hinge joint\n");
    return;
  }



  const vkMatrix4f &globalMatrix = GetLocalTransformation();

  vkMatrix4f frameA, frameB;
  if (colA)
  {
    vkMatrix4f calInv = colA->GetGlobalTransformationInv();
    vkMatrix4f::Mult(calInv, globalMatrix, frameA);
  }
  else
  {
    frameA = globalMatrix;
  }

  if (colB)
  {
    vkMatrix4f calInv = colB->GetGlobalTransformationInv();
    vkMatrix4f::Mult(calInv, globalMatrix, frameB);
  }
  else
  {
    frameB = globalMatrix;
  }

  m_hingeJoint->SetFrames(frameA, frameB);
}