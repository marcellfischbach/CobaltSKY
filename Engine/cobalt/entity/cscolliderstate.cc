
#include <cobalt/entity/cscolliderstate.hh>
#include <cobalt/entity/csentity.hh>
#include <cobalt/entity/csentityscene.hh>
#include <cobalt/physics/iphysicscollider.hh>
#include <cobalt/physics/iphysicsshape.hh>
#include <cobalt/physics/iphysicsscene.hh>
#include <cobalt/csengine.hh>




cs::ColliderState::ColliderState()
  : cs::SpatialState()
  , m_collider(0)
  , m_updateTransformationGuard(false)
{
  
}

cs::ColliderState::~ColliderState()
{
  CS_RELEASE(m_collider);
}

void cs::ColliderState::SetCollider(cs::iPhysicsCollider *collider)
{
  CS_SET(m_collider, collider);
}

void cs::ColliderState::AttachShape(cs::iPhysicsShape* shape)
{
  if (m_collider)
  {
    m_collider->AttachShape(shape);
  }
}

void cs::ColliderState::DetachShape(cs::iPhysicsShape *shape)
{
  if (m_collider)
  {
    m_collider->DetachShape(shape);
  }
}


void cs::ColliderState::AttachShape(cs::PhysicsShape *shapes)
{
  if (m_collider)
  {
    m_collider->AttachShape(shapes);
  }
}



void cs::ColliderState::DetachShape(cs::PhysicsShape *shapes)
{
  if (m_collider)
  {
    m_collider->DetachShape(shapes);
  }
}

void cs::ColliderState::UpdateTransformation()
{

  // don't perform cyclic update with dynmaic bodies
  cs::SpatialState::UpdateTransformation();

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




