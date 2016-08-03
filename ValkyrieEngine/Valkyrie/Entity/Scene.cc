

#include <Valkyrie/Entity/Scene.hh>
#include <Valkyrie/Entity/Entity.hh>
#include <Valkyrie/Entity/SpatialState.hh>
#include <Valkyrie/Engine.hh>

vkEntityScene::vkEntityScene()
  : vkObject()
{
  m_physicsScene = vkEng->CreateScene();
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
  if (!entity)
  {
    return;
  }
  entity->Assemble();
  if (!parent)
  {
    entity->AttachToScene(this);
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
    entity->DetachFromScene(this);
    vkEntity* parent = entity->GetParentEntity();
    if (!parent)
    {
      return;
    }
    parent->DetachEntity(entity);
  }


}

void vkEntityScene::Update(float tpf)
{
  if (m_rootEntity)
  {
    m_rootEntity->Update(tpf);
  }
}

