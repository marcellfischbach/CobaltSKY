
#include <Valkyrie/Entity/RigidBodyState.hh>
#include <Valkyrie/Entity/Entity.hh>
#include <Valkyrie/Physics/IPhysicsBody.hh>
#include <Valkyrie/Physics/IPhysicsShape.hh>
#include <Valkyrie/Engine.hh>

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



vkColliderState::vkColliderState()
  : vkSpatialState()
  , m_collider(0)
  , m_updateTransformationGuard(false)
{
  
}

vkColliderState::~vkColliderState()
{
  VK_RELEASE(m_collider);
}

void vkColliderState::SetCollider(IPhysicsCollider *collider)
{
  VK_SET(m_collider, collider);
}

void vkColliderState::AttachShape(IPhysicsShape* shape)
{
  if (m_collider)
  {
    m_collider->AttachShape(shape);
  }
}

void vkColliderState::DetachShape(IPhysicsShape *shape)
{
  if (m_collider)
  {
    m_collider->DetachShape(shape);
  }
}


void vkColliderState::AttachShape(vkPhysicsShapeContainer *shapes)
{
  if (m_collider)
  {
    m_collider->AttachShape(shapes);
  }
}



void vkColliderState::DetachShape(vkPhysicsShapeContainer *shapes)
{
  if (m_collider)
  {
    m_collider->DetachShape(shapes);
  }
}

void vkColliderState::FinishTransformation()
{
  if (m_updateTransformationGuard)
  {
    return;
  }

  // don't perform cyclic update with dynmaic bodies
  m_updateTransformationGuard = true;
  vkSpatialState::FinishTransformation();

  if (m_collider)
  {
    m_collider->GetTransform().SetTransformation(GetGlobalTransformation());
    m_collider->FinishTransformation();
  }
  m_updateTransformationGuard = false;
}



vkBaseColliderState::vkBaseColliderState()
  : vkColliderState()
  , m_baseCollider(0)
{

}

vkBaseColliderState::~vkBaseColliderState()
{
  VK_RELEASE(m_baseCollider);
}

void vkBaseColliderState::SetBaseCollider(IPhysicsBaseCollider *baseCollider)
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


vkStaticColliderState::vkStaticColliderState()
  : vkBaseColliderState()
  , m_staticCollider(0)
{
  m_staticCollider = vkEngine::Get()->GetPhysicsSystem()->CreateStaticCollider();
  SetBaseCollider(m_staticCollider);
}

vkStaticColliderState::~vkStaticColliderState()
{
  VK_RELEASE(m_staticCollider);
}









vkDynamicColliderState::vkDynamicColliderState()
  : vkBaseColliderState()
  , m_dynamicCollider(0)
{
  m_dynamicCollider = vkEngine::Get()->GetPhysicsSystem()->CreateDynamicCollider();
  SetBaseCollider(m_dynamicCollider);
}

vkDynamicColliderState::~vkDynamicColliderState()
{
  VK_RELEASE(m_dynamicCollider);
}


void vkDynamicColliderState::SetKinematic(bool kinematic)
{
  if (m_dynamicCollider)
  {
    m_dynamicCollider->SetKinematic(kinematic);
  }
}

bool vkDynamicColliderState::IsKinematic() const
{
  return m_dynamicCollider ? m_dynamicCollider->IsKinematic() : false;
}

void vkDynamicColliderState::SetMass(float mass)
{
  if (m_dynamicCollider)
  {
    m_dynamicCollider->SetMass(mass);
  }
}

float vkDynamicColliderState::GetMass() const
{
  return m_dynamicCollider ? m_dynamicCollider->GetMass() : 0.0f;
}

void vkDynamicColliderState::SetInertia(const vkVector3f &inertia)
{
  if (m_dynamicCollider)
  {
    m_dynamicCollider->SetInertia(inertia);
  }
}

const vkVector3f &vkDynamicColliderState::GetInertia() const
{
  if (m_dynamicCollider)
  {
    return m_dynamicCollider->GetInertia();
  }

  static vkVector3f inertia(0.0f, 0.0f, 0.0f);
  return inertia;
}

void vkDynamicColliderState::SetAutoInertia(bool autoInertia)
{
  if (m_dynamicCollider)
  {
    m_dynamicCollider->SetAutoInertia(autoInertia);
  }
}

bool vkDynamicColliderState::IsAutoInertia() const
{
  return m_dynamicCollider ? m_dynamicCollider->IsAutoInertia() : false;
}

void vkDynamicColliderState::DynamicTransformationChanged(const vkMatrix4f &transformation)
{
  GetTransformation().SetTransformation(transformation);
  PerformTransformation();
}