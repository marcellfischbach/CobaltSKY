
#include <cobalt/entity/csstaticcolliderstate.hh>
#include <cobalt/entity/csentityscene.hh>
#include <cobalt/physics/iphysicsstaticcollider.hh>
#include <cobalt/physics/iphysicsscene.hh>
#include <cobalt/csengine.hh>

csStaticColliderState::csStaticColliderState()
  : csBaseColliderState()
  , m_staticCollider(0)
{
  m_staticCollider = csEng->CreateStaticCollider();
  SetBaseCollider(m_staticCollider);
}

csStaticColliderState::~csStaticColliderState()
{
  CS_RELEASE(m_staticCollider);
}




void csStaticColliderState::OnAttachedToScene(csEntityScene *scene)
{
  if (scene)
  {
    scene->GetPhysicsScene()->AddStaticCollider(m_staticCollider);
  }
}

void csStaticColliderState::OnDetachedFromScene(csEntityScene *scene)
{
  if (scene)
  {
    scene->GetPhysicsScene()->RemoveStaticCollider(m_staticCollider);
  }

}


