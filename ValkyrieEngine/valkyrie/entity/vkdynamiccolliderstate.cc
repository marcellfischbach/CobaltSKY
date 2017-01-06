
#include <valkyrie/entity/vkdynamiccolliderstate.hh>
#include <valkyrie/entity/vkentityscene.hh>
#include <valkyrie/entity/itransformationcallback.hh>
#include <valkyrie/physics/iphysicsdynamiccollider.hh>
#include <valkyrie/physics/iphysicsscene.hh>
#include <valkyrie/vkengine.hh>

class vkDynamicColliderStateTransformationCallback : public ITransformationCallback
{
public:
  vkDynamicColliderStateTransformationCallback(vkDynamicColliderState *state)
    : m_state(state)
  {

  }

  virtual void TransformationChanged(const vkMatrix4f &transformation)
  {
    m_state->DynamicTransformationChanged(transformation);
  }


private:
  vkDynamicColliderState *m_state;
};

vkDynamicColliderState::vkDynamicColliderState()
  : vkBaseColliderState()
  , m_dynamicCollider(0)
{
  m_callback = new vkDynamicColliderStateTransformationCallback(this);
  m_dynamicCollider = vkEng->CreateDynamicCollider();
  m_dynamicCollider->SetTransformationCallback(m_callback);
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
  m_updateTransformationGuard = true;
  GetTransformation().SetGlobalTransformation(transformation);
  PerformTransformation();
  m_updateTransformationGuard = false;
}


void vkDynamicColliderState::OnAttachedToScene(vkEntityScene *scene)
{
  if (scene)
  {
    scene->GetPhysicsScene()->AddDynamicCollider(m_dynamicCollider);
  }
}

void vkDynamicColliderState::OnDetachedFromScene(vkEntityScene *scene)
{
  if (scene)
  {
    scene->GetPhysicsScene()->RemoveDynamicCollider(m_dynamicCollider);
  }

}
