

#include <Valkyrie/Entity/Scene.hh>
#include <Valkyrie/Entity/Entity.hh>
#include <Valkyrie/Entity/SpatialState.hh>
#include <Valkyrie/Engine.hh>

vkEntityScene::vkEntityScene()
  : vkObject()
{
  m_physicsScene = vkEngine::Get()->GetPhysicsSystem()->CreateScene();
  m_rootEntity = new vkEntity();
  vkSpatialState *rootState = new vkSpatialState();
  m_rootEntity->SetRootState(rootState);
  m_rootEntity->AddState(rootState);
}

vkEntityScene::~vkEntityScene()
{

}



void vkEntityScene::AddEntity(vkEntity *entity, vkEntity *parent)
{
  if (!parent)
  {
    entity->SetScene(this);
    m_rootEntity->AttachEntity(entity);
  }
  else
  {
    parent->AttachEntity(entity);
  }
}

void vkEntityScene::RemoveEntity(vkEntity *entity)
{
  if (entity->GetScene() == this)
  {
  }
}

