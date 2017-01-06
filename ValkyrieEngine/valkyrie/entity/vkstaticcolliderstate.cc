
#include <valkyrie/entity/vkstaticcolliderstate.hh>
#include <valkyrie/entity/vkentityscene.hh>
#include <valkyrie/physics/iphysicsstaticcollider.hh>
#include <valkyrie/physics/iphysicsscene.hh>
#include <valkyrie/vkengine.hh>

vkStaticColliderState::vkStaticColliderState()
  : vkBaseColliderState()
  , m_staticCollider(0)
{
  m_staticCollider = vkEng->CreateStaticCollider();
  SetBaseCollider(m_staticCollider);
}

vkStaticColliderState::~vkStaticColliderState()
{
  VK_RELEASE(m_staticCollider);
}




void vkStaticColliderState::OnAttachedToScene(vkEntityScene *scene)
{
  if (scene)
  {
    scene->GetPhysicsScene()->AddStaticCollider(m_staticCollider);
  }
}

void vkStaticColliderState::OnDetachedFromScene(vkEntityScene *scene)
{
  if (scene)
  {
    scene->GetPhysicsScene()->RemoveStaticCollider(m_staticCollider);
  }

}


