#pragma once

#include <PhysicsBullet/Export.hh>
#include <Valkyrie/Defs.hh>
#include <Valkyrie/Core/Collection.hh>
#include <Valkyrie/Physics/IPhysicsScene.hh>
#include <PhysicsBullet/BulletScene.refl.hh>
#include <bullet/btBulletDynamicsCommon.h>

class vkBulletBody;

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

  virtual void StepSimulation(float tpf = 1.0f / 60.0f);
  virtual void UpdateEntityTransformation();


  void BodyChanged(vkBulletBody *body);

  btDiscreteDynamicsWorld *GetBulletScene();

private:
  btCollisionConfiguration *m_config;
  btCollisionDispatcher *m_dispatcher;
  btBroadphaseInterface *m_broadphaseInterface;
  btConstraintSolver *m_constraintSolver;
  btDiscreteDynamicsWorld *m_world;

  vkCollection<vkBulletBody*> m_changedBodies;
};

VK_FORCEINLINE btDiscreteDynamicsWorld *vkBulletScene::GetBulletScene()
{
  return m_world;
}


