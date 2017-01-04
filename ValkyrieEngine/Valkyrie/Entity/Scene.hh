#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/vkdefs.hh>
#include <vector>
#include <Valkyrie/Entity/Scene.refl.hh>

struct IPhysicsScene;
class vkEntity;

VK_CLASS()
class VKE_API vkEntityScene : public VK_SUPER(vkObject)
{
  VK_CLASS_GEN;
public:
  vkEntityScene();
  virtual ~vkEntityScene();

  void AddEntity(vkEntity *entity, vkEntity *parent = 0);
  void RemoveEntity(vkEntity *entity);

  vkEntity *GetRoot();
  const vkEntity *GetRoot() const;

  IPhysicsScene *GetPhysicsScene();
  const IPhysicsScene *GetPhysicsScene() const;

  void Update(float tpf);

private:
  vkEntity* m_rootEntity;
  IPhysicsScene *m_physicsScene;

};


VK_FORCEINLINE IPhysicsScene *vkEntityScene::GetPhysicsScene()
{
  return m_physicsScene;
}


VK_FORCEINLINE const IPhysicsScene *vkEntityScene::GetPhysicsScene() const
{
  return m_physicsScene;
}


VK_FORCEINLINE vkEntity *vkEntityScene::GetRoot()
{
  return m_rootEntity;
}

VK_FORCEINLINE const vkEntity *vkEntityScene::GetRoot() const
{
  return m_rootEntity;
}
