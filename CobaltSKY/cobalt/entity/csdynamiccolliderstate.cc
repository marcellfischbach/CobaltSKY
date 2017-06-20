
#include <cobalt/entity/csdynamiccolliderstate.hh>
#include <cobalt/entity/csentityscene.hh>
#include <cobalt/entity/itransformationcallback.hh>
#include <cobalt/physics/iphysicsdynamiccollider.hh>
#include <cobalt/physics/iphysicsscene.hh>
#include <cobalt/csengine.hh>

class csDynamicColliderStateTransformationCallback : public iTransformationCallback
{
public:
  csDynamicColliderStateTransformationCallback(csDynamicColliderState *state)
    : m_state(state)
  {

  }

  virtual void TransformationChanged(const csMatrix4f &transformation)
  {
    m_state->DynamicTransformationChanged(transformation);
  }


private:
  csDynamicColliderState *m_state;
};

csDynamicColliderState::csDynamicColliderState()
  : csBaseColliderState()
  , m_dynamicCollider(0)
{
  m_callback = new csDynamicColliderStateTransformationCallback(this);
  m_dynamicCollider = csEng->CreateDynamicCollider();
  m_dynamicCollider->SetTransformationCallback(m_callback);
  SetBaseCollider(m_dynamicCollider);
}

csDynamicColliderState::~csDynamicColliderState()
{
  CS_RELEASE(m_dynamicCollider);
}


void csDynamicColliderState::SetKinematic(bool kinematic)
{
  if (m_dynamicCollider)
  {
    m_dynamicCollider->SetKinematic(kinematic);
  }
}

bool csDynamicColliderState::IsKinematic() const
{
  return m_dynamicCollider ? m_dynamicCollider->IsKinematic() : false;
}

void csDynamicColliderState::SetMass(float mass)
{
  if (m_dynamicCollider)
  {
    m_dynamicCollider->SetMass(mass);
  }
}

float csDynamicColliderState::GetMass() const
{
  return m_dynamicCollider ? m_dynamicCollider->GetMass() : 0.0f;
}

void csDynamicColliderState::SetInertia(const csVector3f &inertia)
{
  if (m_dynamicCollider)
  {
    m_dynamicCollider->SetInertia(inertia);
  }
}

const csVector3f &csDynamicColliderState::GetInertia() const
{
  if (m_dynamicCollider)
  {
    return m_dynamicCollider->GetInertia();
  }

  static csVector3f inertia(0.0f, 0.0f, 0.0f);
  return inertia;
}

void csDynamicColliderState::SetAutoInertia(bool autoInertia)
{
  if (m_dynamicCollider)
  {
    m_dynamicCollider->SetAutoInertia(autoInertia);
  }
}

bool csDynamicColliderState::IsAutoInertia() const
{
  return m_dynamicCollider ? m_dynamicCollider->IsAutoInertia() : false;
}

void csDynamicColliderState::DynamicTransformationChanged(const csMatrix4f &transformation)
{
  m_updateTransformationGuard = true;
  GetTransformation().SetGlobalTransformation(transformation);
  PerformTransformation();
  m_updateTransformationGuard = false;
}


void csDynamicColliderState::OnAttachedToScene(csEntityScene *scene)
{
  if (scene)
  {
    scene->GetPhysicsScene()->AddDynamicCollider(m_dynamicCollider);
  }
}

void csDynamicColliderState::OnDetachedFromScene(csEntityScene *scene)
{
  if (scene)
  {
    scene->GetPhysicsScene()->RemoveDynamicCollider(m_dynamicCollider);
  }

}
