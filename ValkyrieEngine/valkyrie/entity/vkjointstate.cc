
#include <valkyrie/entity/vkjointstate.hh>
#include <valkyrie/entity/vkdynamiccolliderstate.hh>
#include <valkyrie/entity/vkentityscene.hh>
#include <valkyrie/physics/iphysicsjoint.hh>
#include <valkyrie/physics/iphysicsscene.hh>
#include <valkyrie/physics/iphysicssystem.hh>
#include <valkyrie/vkengine.hh>



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
