
#include <valkyrie/entity/vkbasecolliderstate.hh>
#include <valkyrie/physics/iphysicsbasecollider.hh>


vkBaseColliderState::vkBaseColliderState()
  : vkColliderState()
  , m_baseCollider(0)
{

}

vkBaseColliderState::~vkBaseColliderState()
{
  VK_RELEASE(m_baseCollider);
}

void vkBaseColliderState::SetBaseCollider(iPhysicsBaseCollider *baseCollider)
{
  VK_SET(m_baseCollider, baseCollider);
  SetCollider(m_baseCollider);
}

void vkBaseColliderState::SetFriction(float friction)
{
  if (m_baseCollider)
  {
    m_baseCollider->SetFriction(friction);
  }
}

float vkBaseColliderState::GetFriction() const
{
  if (m_baseCollider)
  {
    return m_baseCollider->GetFriction();
  }
  return 0.0f;
}

void vkBaseColliderState::SetRestitution(float restitution)
{
  if (m_baseCollider)
  {
    m_baseCollider->SetRestitution(restitution);
  }
}

float vkBaseColliderState::GetRestitution() const
{
  if (m_baseCollider)
  {
    return m_baseCollider->GetRestitution();
  }
  return 0.0f;
}
