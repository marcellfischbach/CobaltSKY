
#include <cobalt/entity/csjointstate.hh>
#include <cobalt/entity/csdynamiccolliderstate.hh>
#include <cobalt/entity/csentityscene.hh>
#include <cobalt/physics/iphysicsjoint.hh>
#include <cobalt/physics/iphysicsscene.hh>
#include <cobalt/physics/iphysicssystem.hh>
#include <cobalt/csengine.hh>



cs::JointState::JointState()
  : cs::SpatialState()
  , m_colliderA(0)
  , m_colliderB(0)
  , m_transformReference(eTR_ColliderA)
{

}

cs::JointState::~JointState()
{
  CS_RELEASE(m_colliderA);
  CS_RELEASE(m_colliderB);
}

void cs::JointState::SetColliderA(cs::DynamicColliderState *collider)
{
  CS_SET(m_colliderA, collider);
}

void cs::JointState::SetColliderB(cs::DynamicColliderState *collider)
{
  CS_SET(m_colliderB, collider);
}

void cs::JointState::OnAssembled()
{
  cs::SpatialState::OnAssembled();

  if (m_colliderA)
  {
    m_colliderA->Assemble();
  }
  if (m_colliderB)
  {
    m_colliderB->Assemble();
  }
}
