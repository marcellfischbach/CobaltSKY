
#include <cobalt/entity/csdynamiccolliderstate.hh>
#include <cobalt/entity/csentityscene.hh>
#include <cobalt/entity/itransformationcallback.hh>
#include <cobalt/physics/iphysicsdynamiccollider.hh>
#include <cobalt/physics/iphysicsscene.hh>
#include <cobalt/csengine.hh>

namespace cs
{
class DynamicColliderStateTransformationCallback : public cs::iTransformationCallback
{
public:
  DynamicColliderStateTransformationCallback(cs::DynamicColliderState* state)
    : m_state(state)
  {

  }

  virtual void TransformationChanged(const cs::Matrix4f& transformation)
  {
    m_state->DynamicTransformationChanged(transformation);
  }


private:
  cs::DynamicColliderState* m_state;
};
}

cs::DynamicColliderState::DynamicColliderState()
  : cs::BaseColliderState()
  , m_dynamicCollider(0)
{
  m_callback = new cs::DynamicColliderStateTransformationCallback(this);
  m_dynamicCollider = csEng->CreateDynamicCollider();
  m_dynamicCollider->SetTransformationCallback(m_callback);
  SetBaseCollider(m_dynamicCollider);
}

cs::DynamicColliderState::~DynamicColliderState()
{
  CS_RELEASE(m_dynamicCollider);
}


void cs::DynamicColliderState::SetKinematic(bool kinematic)
{
  if (m_dynamicCollider)
  {
    m_dynamicCollider->SetKinematic(kinematic);
  }
}

bool cs::DynamicColliderState::IsKinematic() const
{
  return m_dynamicCollider ? m_dynamicCollider->IsKinematic() : false;
}

void cs::DynamicColliderState::SetMass(float mass)
{
  m_mass = mass;
  if (m_dynamicCollider)
  {
    m_dynamicCollider->SetMass(mass);
  }
}

float cs::DynamicColliderState::GetMass() const
{
  return m_mass;
  //return m_dynamicCollider ? m_dynamicCollider->GetMass() : 0.0f;
}

void cs::DynamicColliderState::SetInertia(const cs::Vector3f &inertia)
{
  if (m_dynamicCollider)
  {
    m_dynamicCollider->SetInertia(inertia);
  }
}

const cs::Vector3f &cs::DynamicColliderState::GetInertia() const
{
  if (m_dynamicCollider)
  {
    return m_dynamicCollider->GetInertia();
  }

  static cs::Vector3f inertia(0.0f, 0.0f, 0.0f);
  return inertia;
}

void cs::DynamicColliderState::SetAutoInertia(bool autoInertia)
{
  if (m_dynamicCollider)
  {
    m_dynamicCollider->SetAutoInertia(autoInertia);
  }
}

bool cs::DynamicColliderState::IsAutoInertia() const
{
  return m_dynamicCollider ? m_dynamicCollider->IsAutoInertia() : false;
}

void cs::DynamicColliderState::DynamicTransformationChanged(const cs::Matrix4f &transformation)
{
  m_updateTransformationGuard = true;
  GetTransformation().SetGlobalTransformation(transformation);
  PerformTransformation();
  m_updateTransformationGuard = false;
}


void cs::DynamicColliderState::OnAttachedToScene(cs::EntityScene *scene)
{
  if (scene)
  {
    scene->GetPhysicsScene()->AddDynamicCollider(m_dynamicCollider);
  }
}

void cs::DynamicColliderState::OnDetachedFromScene(cs::EntityScene *scene)
{
  if (scene)
  {
    scene->GetPhysicsScene()->RemoveDynamicCollider(m_dynamicCollider);
  }

}
