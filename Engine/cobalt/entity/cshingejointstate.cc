
#include <cobalt/entity/cshingejointstate.hh>
#include <cobalt/entity/csdynamiccolliderstate.hh>
#include <cobalt/entity/csentityscene.hh>
#include <cobalt/physics/iphysicscollider.hh>
#include <cobalt/physics/iphysicshingejoint.hh>
#include <cobalt/physics/iphysicsscene.hh>
#include <cobalt/csengine.hh>



cs::HingeJointState::HingeJointState()
  : cs::JointState()
  , m_hingeJoint(0)
{

}

cs::HingeJointState::~HingeJointState()
{

}

void cs::HingeJointState::OnAssembled()
{
  // make all connected joints be assembled
  cs::JointState::OnAssembled();

  // create the joint
  cs::DynamicColliderState *colA = GetColliderA();
  cs::DynamicColliderState *colB = GetColliderB();
  cs::iPhysicsDynamicCollider *colliderA = colA ? colA->GetDynamicCollider() : 0;
  cs::iPhysicsDynamicCollider *colliderB = colB ? colB->GetDynamicCollider() : 0;
  cs::iPhysicsJoint *joint = csEng->CreateJoint(cs::ePJT_Hinge, colliderA, colliderB);
  m_hingeJoint = cs::QueryClass<cs::iPhysicsHingeJoint>(joint);
  if (!m_hingeJoint)
  {
    return;
  }



  const cs::Matrix4f &globalMatrix = GetGlobalTransformation();

  cs::Matrix4f frameA, frameB;
  if (colA)
  {
    cs::Matrix4f calInv = colA->GetGlobalTransformationInv();
    cs::Matrix4f::Mult(calInv, globalMatrix, frameA);
  }
  else
  {
    frameA = globalMatrix;
  }

  if (colB)
  {
    cs::Matrix4f calInv = colB->GetGlobalTransformationInv();
    cs::Matrix4f::Mult(calInv, globalMatrix, frameB);
  }
  else
  {
    frameB = globalMatrix;
  }

  m_hingeJoint->SetFrames(frameA, frameB);

}

void cs::HingeJointState::OnAttachedToScene(cs::EntityScene *scene)
{
  if (m_hingeJoint && scene)
  {
    scene->GetPhysicsScene()->AddJoint(m_hingeJoint);
  }
}

void cs::HingeJointState::OnDetachedFromScene(cs::EntityScene *scene)
{
  if (m_hingeJoint && scene)
  {
    scene->GetPhysicsScene()->RemoveJoint(m_hingeJoint);
  }
}


void cs::HingeJointState::UpdateTransformation()
{
  cs::SpatialState::UpdateTransformation();

  if (!m_hingeJoint)
  {
    return;
  }

  cs::DynamicColliderState *colA = GetColliderA();
  cs::DynamicColliderState *colB = GetColliderB();
  if (colA && colB)
  {
    return;
  }

  const cs::Matrix4f &globalMatrix = GetGlobalTransformation();

  cs::Matrix4f frameA, frameB;
  if (!colA)
  {
    m_hingeJoint->SetFrameA(globalMatrix);
  }

  if (!colB)
  {
    m_hingeJoint->SetFrameB(globalMatrix);
  }

}

