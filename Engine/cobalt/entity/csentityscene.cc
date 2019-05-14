

#include <cobalt/entity/csentityscene.hh>
#include <cobalt/entity/csentity.hh>
#include <cobalt/entity/csspatialstate.hh>
#include <cobalt/csengine.hh>

csEntityScene::csEntityScene()
  : cs::Object()
{
  m_physicsScene = csEng->CreateScene();
  m_rootEntity = new csEntity();
  csSpatialState *rootState = new csSpatialState();
  m_rootEntity->SetRootState(rootState);
  m_rootEntity->AddState(rootState);
}

csEntityScene::~csEntityScene()
{

}



void csEntityScene::AddEntity(csEntity *entity, csEntity *parent)
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

void csEntityScene::RemoveEntity(csEntity *entity)
{
  if (entity->GetScene() == this)
  {
    entity->DetachFromScene(this);
    csEntity* parent = entity->GetParentEntity();
    if (!parent)
    {
      return;
    }
    parent->DetachEntity(entity);
  }


}

void csEntityScene::Update(float tpf)
{
  if (m_rootEntity)
  {
    m_rootEntity->Update(tpf);
  }
}

