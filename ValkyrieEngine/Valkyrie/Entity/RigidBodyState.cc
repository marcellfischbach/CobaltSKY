
#include <Valkyrie/Entity/RigidBodyState.hh>
#include <Valkyrie/Entity/Entity.hh>
#include <Valkyrie/Physics/IPhysicsBody.hh>


vkRigidBodyState::vkRigidBodyState()
  : vkEntityState ()
  , m_mode(ePBM_Static)
  , m_mass(0.0)
  , m_friction (0.5f)
  , m_restitution(0.5f)
{

}

vkRigidBodyState::~vkRigidBodyState()
{

}

void vkRigidBodyState::UpdateBody()
{
  IPhysicsBody *body = GetEntity()->AcquireCollisionBody();
  body->SetMode(m_mode);
  body->SetMass(m_mass);
  body->SetFriction(m_friction);
  body->SetRestitution(m_restitution);
  body->SetAutoInertia(true);
  body->Update();
}

void vkRigidBodyState::OnAttachedToEntity(vkEntity *entity)
{
  vkEntityState::OnAttachedToEntity(entity);

  UpdateBody();
}