
#include <cobalt/entity/csbasecolliderstate.hh>
#include <cobalt/physics/iphysicsbasecollider.hh>


csBaseColliderState::csBaseColliderState()
  : csColliderState()
  , m_baseCollider(0)
{

}

csBaseColliderState::~csBaseColliderState()
{
  CS_RELEASE(m_baseCollider);
}

void csBaseColliderState::SetBaseCollider(iPhysicsBaseCollider *baseCollider)
{
  CS_SET(m_baseCollider, baseCollider);
  SetCollider(m_baseCollider);
}

void csBaseColliderState::SetFriction(float friction)
{
  if (m_baseCollider)
  {
    m_baseCollider->SetFriction(friction);
  }
}

float csBaseColliderState::GetFriction() const
{
  if (m_baseCollider)
  {
    return m_baseCollider->GetFriction();
  }
  return 0.0f;
}

void csBaseColliderState::SetRestitution(float restitution)
{
  if (m_baseCollider)
  {
    m_baseCollider->SetRestitution(restitution);
  }
}

float csBaseColliderState::GetRestitution() const
{
  if (m_baseCollider)
  {
    return m_baseCollider->GetRestitution();
  }
  return 0.0f;
}
