
#include <Valkyrie/entity/vkjointstate.hh>
#include <Valkyrie/entity/vkdynamiccolliderstate.hh>
#include <Valkyrie/entity/vkentityscene.hh>
#include <Valkyrie/Physics/IPhysicsJoints.hh>
#include <Valkyrie/Physics/IPhysicsScene.hh>
#include <Valkyrie/Physics/IPhysicsSystem.hh>
#include <Valkyrie/vkengine.hh>



vkJointState::vkJointState()
  : vkSpatialState()
  , m_colliderA(0)
  , m_colliderB(0)
  , m_transformReference(eTR_ColliderA)
{

}

vkJointState::~vkJointState()
{
  VK_RELEASE(m_colliderA);
  VK_RELEASE(m_colliderB);
}

void vkJointState::SetColliderA(vkDynamicColliderState *collider)
{
  VK_SET(m_colliderA, collider);
}

void vkJointState::SetColliderB(vkDynamicColliderState *collider)
{
  VK_SET(m_colliderB, collider);
}

void vkJointState::OnAssembled()
{
  vkSpatialState::OnAssembled();

  if (m_colliderA)
  {
    m_colliderA->Assemble();
  }
  if (m_colliderB)
  {
    m_colliderB->Assemble();
  }
}
