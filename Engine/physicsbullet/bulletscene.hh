#pragma once

#include <physicsbullet/bulletexport.hh>
#include <cobalt/csdefs.hh>
#include <cobalt/core/cscollection.hh>
#include <cobalt/physics/iphysicsscene.hh>
#include <bullet/btBulletDynamicsCommon.h>
#include <physicsbullet/bulletscene.refl.hh>

namespace cs
{
class BulletBody;
class BulletDynamicCollider;
class BulletCapsuleCharacterController;

CS_CLASS()
class CSBULLETPHYSICS_API BulletScene : public CS_SUPER(cs::iPhysicsScene)
{
  CS_CLASS_GEN_OBJECT;
public:
  BulletScene();
  virtual ~BulletScene();

  bool Initialize(bool softBody);

  virtual void AddCollider(cs::iPhysicsCollider * collider);
  virtual void RemoveCollider(cs::iPhysicsCollider * collider);

  virtual void AddStaticCollider(cs::iPhysicsStaticCollider * collider);
  virtual void RemoveStaticCollider(cs::iPhysicsStaticCollider * collider);

  virtual void AddDynamicCollider(cs::iPhysicsDynamicCollider * collider);
  virtual void RemoveDynamicCollider(cs::iPhysicsDynamicCollider * collider);

  virtual void AddCharacterController(cs::iPhysicsCharacterController * controller);
  virtual void RemoveCharacterController(cs::iPhysicsCharacterController * controller);

  virtual void AddJoint(cs::iPhysicsJoint * joint);
  virtual void RemoveJoint(cs::iPhysicsJoint * joint);

  virtual void StepSimulation(float tpf = 1.0f / 60.0f);
  virtual void UpdateColliders();


  void BodyChanged(cs::BulletBody * body);
  void DynamicColliderChanged(cs::BulletDynamicCollider * dynamicCollider);

  btDiscreteDynamicsWorld * GetBulletScene();

private:
  btCollisionConfiguration * m_config;
  btCollisionDispatcher * m_dispatcher;
  btBroadphaseInterface * m_broadphaseInterface;
  btConstraintSolver * m_constraintSolver;
  btDiscreteDynamicsWorld * m_world;

  cs::Collection<cs::BulletBody*> m_changedBodies;
  cs::Collection<cs::BulletDynamicCollider*> m_changedDynamicColliders;
  std::vector<cs::BulletCapsuleCharacterController*> m_characterControllers;
};

}

CS_FORCEINLINE btDiscreteDynamicsWorld *cs::BulletScene::GetBulletScene()
{
  return m_world;
}


