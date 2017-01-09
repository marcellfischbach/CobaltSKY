
#include <valkyrie/entity/vkhingejointstate.hh>
#include <valkyrie/entity/vkdynamiccolliderstate.hh>
#include <valkyrie/entity/vkentityscene.hh>
#include <valkyrie/physics/iphysicscollider.hh>
#include <valkyrie/physics/iphysicshingejoint.hh>
#include <valkyrie/physics/iphysicsscene.hh>
#include <valkyrie/vkengine.hh>



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
  iPhysicsDynamicCollider *colliderA = colA ? colA->GetDynamicCollider() : 0;
  iPhysicsDynamicCollider *colliderB = colB ? colB->GetDynamicCollider() : 0;
  iPhysicsJoint *joint = vkEng->CreateJoint(ePJT_Hinge, colliderA, colliderB);
  m_hingeJoint = vkQueryClass<iPhysicsHingeJoint>(joint);
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

