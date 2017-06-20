#pragma once

#include <physicsbullet/bulletexport.hh>
#include <cobalt/csdefs.hh>
#include <cobalt/core/cscollection.hh>
#include <cobalt/physics/iphysicsscene.hh>
#include <bullet/btBulletDynamicsCommon.h>
#include <physicsbullet/bulletscene.refl.hh>

class csBulletBody;
class csBulletDynamicCollider;
class csBulletCapsuleCharacterController;

CS_CLASS()
class CSBULLETPHYSICS_API csBulletScene : public CS_SUPER(iPhysicsScene)
{
  CS_CLASS_GEN_OBJECT;
public:
  csBulletScene();
  virtual ~csBulletScene();

  bool Initialize(bool softBody);


  virtual void AddStaticCollider(iPhysicsStaticCollider *collider);
  virtual void RemoveStaticCollider(iPhysicsStaticCollider *collider);

  virtual void AddDynamicCollider(iPhysicsDynamicCollider *collider);
  virtual void RemoveDynamicCollider(iPhysicsDynamicCollider *collider);

  virtual void AddCharacterController(iPhysicsCharacterController *controller);
  virtual void RemoveCharacterController(iPhysicsCharacterController *controller);

  virtual void AddJoint(iPhysicsJoint *joint);
  virtual void RemoveJoint(iPhysicsJoint *joint);

  virtual void StepSimulation(float tpf = 1.0f / 60.0f);
  virtual void UpdateColliders();


  void BodyChanged(csBulletBody *body);
  void DynamicColliderChanged(csBulletDynamicCollider *dynamicCollider);

  btDiscreteDynamicsWorld *GetBulletScene();

private:
  btCollisionConfiguration *m_config;
  btCollisionDispatcher *m_dispatcher;
  btBroadphaseInterface *m_broadphaseInterface;
  btConstraintSolver *m_constraintSolver;
  btDiscreteDynamicsWorld *m_world;

  csCollection<csBulletBody*> m_changedBodies;
  csCollection<csBulletDynamicCollider*> m_changedDynamicColliders;
  std::vector<csBulletCapsuleCharacterController*> m_characterControllers;
};

CS_FORCEINLINE btDiscreteDynamicsWorld *csBulletScene::GetBulletScene()
{
  return m_world;
}


