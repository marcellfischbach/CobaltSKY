#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/csdefs.hh>
#include <vector>
#include <cobalt/entity/csentityscene.refl.hh>

struct iPhysicsScene;
class csEntity;

CS_CLASS()
class CSE_API csEntityScene : public CS_SUPER(csObject)
{
  CS_CLASS_GEN;
public:
  csEntityScene();
  virtual ~csEntityScene();

  void AddEntity(csEntity *entity, csEntity *parent = 0);
  void RemoveEntity(csEntity *entity);

  csEntity *GetRoot();
  const csEntity *GetRoot() const;

  iPhysicsScene *GetPhysicsScene();
  const iPhysicsScene *GetPhysicsScene() const;

  void Update(float tpf);

private:
  csEntity* m_rootEntity;
  iPhysicsScene *m_physicsScene;

};


CS_FORCEINLINE iPhysicsScene *csEntityScene::GetPhysicsScene()
{
  return m_physicsScene;
}


CS_FORCEINLINE const iPhysicsScene *csEntityScene::GetPhysicsScene() const
{
  return m_physicsScene;
}


CS_FORCEINLINE csEntity *csEntityScene::GetRoot()
{
  return m_rootEntity;
}

CS_FORCEINLINE const csEntity *csEntityScene::GetRoot() const
{
  return m_rootEntity;
}
