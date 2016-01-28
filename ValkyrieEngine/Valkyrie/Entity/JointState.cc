
#include <Valkyrie/Entity/JointState.hh>
#include <Valkyrie/Entity/ColliderState.hh>
#include <Valkyrie/Entity/Scene.hh>
#include <Valkyrie/Physics/IPhysicsJoints.hh>
#include <Valkyrie/Physics/IPhysicsScene.hh>
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

void vkJointState::OnAssembled()
{
  vkSpatialState::OnAssembled();

  if (m_colliderA)
  {
    m_colliderA->Assemble();
  }
  if (m_colliderB)
  {
    m_colliderB->Assemble();
  }
}


vkHingeJointState::vkHingeJointState()
  : vkJointState()
  , m_hingeJoint(0)
{

}

vkHingeJointState::~vkHingeJointState()
{

}

void vkHingeJointState::OnAssembled()
{
  // make all connected joints be assembled
  vkJointState::OnAssembled();

  // create the joint
  vkDynamicColliderState *colA = GetColliderA();
  vkDynamicColliderState *colB = GetColliderB();
  IPhysicsDynamicCollider *colliderA = colA ? colA->GetDynamicCollider() : 0;
  IPhysicsDynamicCollider *colliderB = colB ? colB->GetDynamicCollider() : 0;
  IPhysicsJoint *joint = vkEngine::Get()->GetPhysicsSystem()->CreateJoint(ePJT_Hinge, colliderA, colliderB);
  m_hingeJoint = vkQueryClass<IPhysicsHingeJoint>(joint);
  if (!m_hingeJoint)
  {
    return;
  }



  const vkMatrix4f &globalMatrix = GetGlobalTransformation();

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

void vkHingeJointState::OnAttachedToScene(vkEntityScene *scene)
{
  if (m_hingeJoint && scene)
  {
    scene->GetPhysicsScene()->AddJoint(m_hingeJoint);
  }
}

void vkHingeJointState::OnDetachedFromScene(vkEntityScene *scene)
{
  if (m_hingeJoint && scene)
  {
    scene->GetPhysicsScene()->RemoveJoint(m_hingeJoint);
  }
}


void vkHingeJointState::UpdateTransformation()
{
  vkSpatialState::UpdateTransformation();

  if (!m_hingeJoint)
  {
    return;
  }

  vkDynamicColliderState *colA = GetColliderA();
  vkDynamicColliderState *colB = GetColliderB();
  if (colA && colB)
  {
    return;
  }

  const vkMatrix4f &globalMatrix = GetGlobalTransformation();

  vkMatrix4f frameA, frameB;
  if (!colA)
  {
    m_hingeJoint->SetFrameA(globalMatrix);
  }
  
  if (!colB)
  {
    m_hingeJoint->SetFrameB(globalMatrix);
  }

}

