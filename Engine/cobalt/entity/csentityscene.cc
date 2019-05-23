

#include <cobalt/entity/csentityscene.hh>
#include <cobalt/entity/csentity.hh>
#include <cobalt/entity/csspatialstate.hh>
#include <cobalt/csengine.hh>

cs::EntityScene::EntityScene()
  : cs::Object()
{
  m_physicsScene = csEng->CreateScene();
  m_rootEntity = new cs::Entity();
  cs::SpatialState *rootState = new cs::SpatialState();
  m_rootEntity->SetRootState(rootState);
  m_rootEntity->AddState(rootState);
}

cs::EntityScene::~EntityScene()
{

}



void cs::EntityScene::AddEntity(cs::Entity *entity, cs::Entity *parent)
{
  if (!entity)
  {
    return;
  }
  if (entity->GetScene() == this)
  {
    // don't attach the enityt to the same scene twice
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

void cs::EntityScene::RemoveEntity(cs::Entity *entity)
{
  if (entity->GetScene() == this)
  {
    entity->DetachFromScene(this);
    cs::Entity* parent = entity->GetParentEntity();
    if (!parent)
    {
      return;
    }
    parent->DetachEntity(entity);
  }


}

void cs::EntityScene::Update(float tpf)
{
  if (m_rootEntity)
  {
    m_rootEntity->Update(tpf);
  }
}

