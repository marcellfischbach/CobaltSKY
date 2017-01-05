
#include <Valkyrie/entity/vkstaticcolliderstate.hh>
#include <Valkyrie/entity/vkentityscene.hh>
#include <Valkyrie/Physics/iphysicsstaticcollider.hh>
#include <Valkyrie/Physics/iphysicsscene.hh>
#include <Valkyrie/vkengine.hh>

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


