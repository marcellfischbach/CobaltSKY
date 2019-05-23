
#include <cobalt/entity/csbasecolliderstate.hh>
#include <cobalt/physics/iphysicsbasecollider.hh>


cs::BaseColliderState::BaseColliderState()
  : cs::ColliderState()
  , m_baseCollider(0)
{

}

cs::BaseColliderState::~BaseColliderState()
{
  CS_RELEASE(m_baseCollider);
}

void cs::BaseColliderState::SetBaseCollider(cs::iPhysicsBaseCollider *baseCollider)
{
  CS_SET(m_baseCollider, baseCollider);
  SetCollider(m_baseCollider);
}

void cs::BaseColliderState::SetFriction(float friction)
{
  if (m_baseCollider)
  {
    m_baseCollider->SetFriction(friction);
  }
}

float cs::BaseColliderState::GetFriction() const
{
  if (m_baseCollider)
  {
    return m_baseCollider->GetFriction();
  }
  return 0.0f;
}

void cs::BaseColliderState::SetRestitution(float restitution)
{
  if (m_baseCollider)
  {
    m_baseCollider->SetRestitution(restitution);
  }
}

float cs::BaseColliderState::GetRestitution() const
{
  if (m_baseCollider)
  {
    return m_baseCollider->GetRestitution();
  }
  return 0.0f;
}
