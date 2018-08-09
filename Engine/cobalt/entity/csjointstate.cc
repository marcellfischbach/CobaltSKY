
#include <cobalt/entity/csjointstate.hh>
#include <cobalt/entity/csdynamiccolliderstate.hh>
#include <cobalt/entity/csentityscene.hh>
#include <cobalt/physics/iphysicsjoint.hh>
#include <cobalt/physics/iphysicsscene.hh>
#include <cobalt/physics/iphysicssystem.hh>
#include <cobalt/csengine.hh>



csJointState::csJointState()
  : csSpatialState()
  , m_colliderA(0)
  , m_colliderB(0)
  , m_transformReference(eTR_ColliderA)
{

}

csJointState::~csJointState()
{
  CS_RELEASE(m_colliderA);
  CS_RELEASE(m_colliderB);
}

void csJointState::SetColliderA(csDynamicColliderState *collider)
{
  CS_SET(m_colliderA, collider);
}

void csJointState::SetColliderB(csDynamicColliderState *collider)
{
  CS_SET(m_colliderB, collider);
}

void csJointState::OnAssembled()
{
  csSpatialState::OnAssembled();

  if (m_colliderA)
  {
    m_colliderA->Assemble();
  }
  if (m_colliderB)
  {
    m_colliderB->Assemble();
  }
}
