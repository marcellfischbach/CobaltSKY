#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/csdefs.hh>
#include <vector>
#include <cobalt/entity/csentityscene.refl.hh>


namespace cs
{
struct iPhysicsScene;
class Entity;

CS_CLASS()
class CSE_API EntityScene : public CS_SUPER(cs::Object)
{
  CS_CLASS_GEN;
public:
  EntityScene();
  virtual ~EntityScene();

  void AddEntity(cs::Entity * entity, cs::Entity * parent = 0);
  void RemoveEntity(cs::Entity * entity);

  cs::Entity * GetRoot();
  const cs::Entity * GetRoot() const;

  cs::iPhysicsScene * GetPhysicsScene();
  const cs::iPhysicsScene * GetPhysicsScene() const;

  void Update(float tpf);

private:
  cs::Entity * m_rootEntity;
  cs::iPhysicsScene * m_physicsScene;

};

}

CS_FORCEINLINE cs::iPhysicsScene *cs::EntityScene::GetPhysicsScene()
{
  return m_physicsScene;
}


CS_FORCEINLINE const cs::iPhysicsScene *cs::EntityScene::GetPhysicsScene() const
{
  return m_physicsScene;
}


CS_FORCEINLINE cs::Entity *cs::EntityScene::GetRoot()
{
  return m_rootEntity;
}

CS_FORCEINLINE const cs::Entity *cs::EntityScene::GetRoot() const
{
  return m_rootEntity;
}
