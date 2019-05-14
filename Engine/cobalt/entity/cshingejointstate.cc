
#include <cobalt/entity/cshingejointstate.hh>
#include <cobalt/entity/csdynamiccolliderstate.hh>
#include <cobalt/entity/csentityscene.hh>
#include <cobalt/physics/iphysicscollider.hh>
#include <cobalt/physics/iphysicshingejoint.hh>
#include <cobalt/physics/iphysicsscene.hh>
#include <cobalt/csengine.hh>



csHingeJointState::csHingeJointState()
  : csJointState()
  , m_hingeJoint(0)
{

}

csHingeJointState::~csHingeJointState()
{

}

void csHingeJointState::OnAssembled()
{
  // make all connected joints be assembled
  csJointState::OnAssembled();

  // create the joint
  csDynamicColliderState *colA = GetColliderA();
  csDynamicColliderState *colB = GetColliderB();
  iPhysicsDynamicCollider *colliderA = colA ? colA->GetDynamicCollider() : 0;
  iPhysicsDynamicCollider *colliderB = colB ? colB->GetDynamicCollider() : 0;
  iPhysicsJoint *joint = csEng->CreateJoint(ePJT_Hinge, colliderA, colliderB);
  m_hingeJoint = cs::QueryClass<iPhysicsHingeJoint>(joint);
  if (!m_hingeJoint)
  {
    return;
  }



  const csMatrix4f &globalMatrix = GetGlobalTransformation();

  csMatrix4f frameA, frameB;
  if (colA)
  {
    csMatrix4f calInv = colA->GetGlobalTransformationInv();
    csMatrix4f::Mult(calInv, globalMatrix, frameA);
  }
  else
  {
    frameA = globalMatrix;
  }

  if (colB)
  {
    csMatrix4f calInv = colB->GetGlobalTransformationInv();
    csMatrix4f::Mult(calInv, globalMatrix, frameB);
  }
  else
  {
    frameB = globalMatrix;
  }

  m_hingeJoint->SetFrames(frameA, frameB);

}

void csHingeJointState::OnAttachedToScene(csEntityScene *scene)
{
  if (m_hingeJoint && scene)
  {
    scene->GetPhysicsScene()->AddJoint(m_hingeJoint);
  }
}

void csHingeJointState::OnDetachedFromScene(csEntityScene *scene)
{
  if (m_hingeJoint && scene)
  {
    scene->GetPhysicsScene()->RemoveJoint(m_hingeJoint);
  }
}


void csHingeJointState::UpdateTransformation()
{
  csSpatialState::UpdateTransformation();

  if (!m_hingeJoint)
  {
    return;
  }

  csDynamicColliderState *colA = GetColliderA();
  csDynamicColliderState *colB = GetColliderB();
  if (colA && colB)
  {
    return;
  }

  const csMatrix4f &globalMatrix = GetGlobalTransformation();

  csMatrix4f frameA, frameB;
  if (!colA)
  {
    m_hingeJoint->SetFrameA(globalMatrix);
  }

  if (!colB)
  {
    m_hingeJoint->SetFrameB(globalMatrix);
  }

}

