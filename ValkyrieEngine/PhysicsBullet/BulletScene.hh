#pragma once

#include <PhysicsBullet/Export.hh>
#include <Valkyrie/Defs.hh>
#include <Valkyrie/Core/Collection.hh>
#include <Valkyrie/Physics/IPhysicsScene.hh>
#include <PhysicsBullet/BulletScene.refl.hh>
#include <bullet/btBulletDynamicsCommon.h>

class vkBulletBody;
class vkBulletDynamicCollider;

VK_CLASS()
class VKBULLET_API vkBulletScene : public IPhysicsScene
{
  VK_CLASS_GEN_OBJECT;
public:
  vkBulletScene();
  virtual ~vkBulletScene();

  bool Initialize(bool softBody);


  virtual void AddBody(IPhysicsBody *body);
  virtual void RemoveBody(IPhysicsBody *body);

  virtual void AddStaticCollider(IPhysicsStaticCollider *collider);
  virtual void RemoveStaticCollider(IPhysicsStaticCollider *collider);

  virtual void AddDynamicCollider(IPhysicsDynamicCollider *collider);
  virtual void RemoveDynamicCollider(IPhysicsDynamicCollider *collider);

  virtual void StepSimulation(float tpf = 1.0f / 60.0f);
  virtual void UpdateColliders();


  void BodyChanged(vkBulletBody *body);
  void DynamicColliderChanged(vkBulletDynamicCollider *dynamicCollider);

  btDiscreteDynamicsWorld *GetBulletScene();

private:
  btCollisionConfiguration *m_config;
  btCollisionDispatcher *m_dispatcher;
  btBroadphaseInterface *m_broadphaseInterface;
  btConstraintSolver *m_constraintSolver;
  btDiscreteDynamicsWorld *m_world;

  vkCollection<vkBulletBody*> m_changedBodies;
  vkCollection<vkBulletDynamicCollider*> m_changedDynamicColliders;
};

VK_FORCEINLINE btDiscreteDynamicsWorld *vkBulletScene::GetBulletScene()
{
  return m_world;
}


