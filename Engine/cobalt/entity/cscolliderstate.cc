
#include <cobalt/entity/cscolliderstate.hh>
#include <cobalt/entity/csentity.hh>
#include <cobalt/entity/csentityscene.hh>
#include <cobalt/physics/iphysicscollider.hh>
#include <cobalt/physics/iphysicsshape.hh>
#include <cobalt/physics/iphysicsscene.hh>
#include <cobalt/csengine.hh>




csColliderState::csColliderState()
  : csSpatialState()
  , m_collider(0)
  , m_updateTransformationGuard(false)
{
  
}

csColliderState::~csColliderState()
{
  CS_RELEASE(m_collider);
}

void csColliderState::SetCollider(iPhysicsCollider *collider)
{
  CS_SET(m_collider, collider);
}

void csColliderState::AttachShape(iPhysicsShape* shape)
{
  if (m_collider)
  {
    m_collider->AttachShape(shape);
  }
}

void csColliderState::DetachShape(iPhysicsShape *shape)
{
  if (m_collider)
  {
    m_collider->DetachShape(shape);
  }
}


void csColliderState::AttachShape(csPhysicsShape *shapes)
{
  if (m_collider)
  {
    m_collider->AttachShape(shapes);
  }
}



void csColliderState::DetachShape(csPhysicsShape *shapes)
{
  if (m_collider)
  {
    m_collider->DetachShape(shapes);
  }
}

void csColliderState::UpdateTransformation()
{

  // don't perform cyclic update with dynmaic bodies
  csSpatialState::UpdateTransformation();

  if (m_updateTransformationGuard)
  {
    return;
  }
  m_updateTransformationGuard = true;
  if (m_collider)
  {
    m_collider->GetTransform().SetTransformation(GetGlobalTransformation());
    m_collider->FinishTransformation();
  }
  m_updateTransformationGuard = false;
}




