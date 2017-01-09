#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/vkdefs.hh>
#include <vector>
#include <valkyrie/entity/vkentityscene.refl.hh>

struct iPhysicsScene;
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

  iPhysicsScene *GetPhysicsScene();
  const iPhysicsScene *GetPhysicsScene() const;

  void Update(float tpf);

private:
  vkEntity* m_rootEntity;
  iPhysicsScene *m_physicsScene;

};


VK_FORCEINLINE iPhysicsScene *vkEntityScene::GetPhysicsScene()
{
  return m_physicsScene;
}


VK_FORCEINLINE const iPhysicsScene *vkEntityScene::GetPhysicsScene() const
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
