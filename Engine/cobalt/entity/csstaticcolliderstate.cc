
#include <cobalt/entity/csstaticcolliderstate.hh>
#include <cobalt/entity/csentityscene.hh>
#include <cobalt/physics/iphysicsstaticcollider.hh>
#include <cobalt/physics/iphysicsscene.hh>
#include <cobalt/csengine.hh>

cs::StaticColliderState::StaticColliderState()
  : cs::BaseColliderState()
  , m_staticCollider(0)
{
  m_staticCollider = csEng->CreateStaticCollider();
  SetBaseCollider(m_staticCollider);
}

cs::StaticColliderState::~StaticColliderState ()
{
  CS_RELEASE(m_staticCollider);
}




void cs::StaticColliderState::OnAttachedToScene(cs::EntityScene *scene)
{
  if (scene)
  {
    scene->GetPhysicsScene()->AddStaticCollider(m_staticCollider);
  }
}

void cs::StaticColliderState::OnDetachedFromScene(cs::EntityScene *scene)
{
  if (scene)
  {
    scene->GetPhysicsScene()->RemoveStaticCollider(m_staticCollider);
  }

}


