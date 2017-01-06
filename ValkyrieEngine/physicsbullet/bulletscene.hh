#pragma once

#include <physicsbullet/bulletexport.hh>
#include <valkyrie/vkdefs.hh>
#include <valkyrie/core/vkcollection.hh>
#include <valkyrie/physics/iphysicsscene.hh>
#include <bullet/btBulletDynamicsCommon.h>
#include <physicsbullet/bulletscene.refl.hh>

class vkBulletBody;
class vkBulletDynamicCollider;
class vkBulletCapsuleCharacterController;

VK_CLASS()
class VKBULLETPHYSICS_API vkBulletScene : public VK_SUPER(IPhysicsScene)
{
  VK_CLASS_GEN_OBJECT;
public:
  vkBulletScene();
  virtual ~vkBulletScene();

  bool Initialize(bool softBody);


  virtual void AddStaticCollider(IPhysicsStaticCollider *collider);
  virtual void RemoveStaticCollider(IPhysicsStaticCollider *collider);

  virtual void AddDynamicCollider(IPhysicsDynamicCollider *collider);
  virtual void RemoveDynamicCollider(IPhysicsDynamicCollider *collider);

  virtual void AddCharacterController(IPhysicsCharacterController *controller);
  virtual void RemoveCharacterController(IPhysicsCharacterController *controller);

  virtual void AddJoint(IPhysicsJoint *joint);
  virtual void RemoveJoint(IPhysicsJoint *joint);

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
  std::vector<vkBulletCapsuleCharacterController*> m_characterControllers;
};

VK_FORCEINLINE btDiscreteDynamicsWorld *vkBulletScene::GetBulletScene()
{
  return m_world;
}

