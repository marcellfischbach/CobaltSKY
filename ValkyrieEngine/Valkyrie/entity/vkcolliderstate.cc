
#include <Valkyrie/entity/vkcolliderstate.hh>
#include <Valkyrie/entity/vkentity.hh>
#include <Valkyrie/entity/vkentityscene.hh>
#include <Valkyrie/physics/iphysicscollider.hh>
#include <Valkyrie/physics/iphysicsshape.hh>
#include <Valkyrie/physics/iphysicsscene.hh>
#include <Valkyrie/vkengine.hh>




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

void vkColliderState::UpdateTransformation()
{

  // don't perform cyclic update with dynmaic bodies
  vkSpatialState::UpdateTransformation();

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




