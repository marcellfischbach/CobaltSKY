#pragma once

#include <PhysicsBullet/Export.hh>
#include <Valkyrie/Entity/Entity.hh>
#include <Valkyrie/Physics/IPhysicsBody.hh>
#include <PhysicsBullet/BulletBody.refl.hh>
#include <vector>
#include <bullet/btBulletDynamicsCommon.h>

class btCollisionShape;
class btCompoundShape;
class vkBulletShape;
class vkBulletScene;

class vkBulletBody;
class vkBulletBodyImpl : public btMotionState
{
public:
  vkBulletBodyImpl(vkBulletBody *body);
  ~vkBulletBodyImpl();

  void AttachShape(vkBulletShape *shape);
  bool DetachShape(vkBulletShape *shape);

  void AttachToScene(vkBulletScene *scene);
  void DetachFromScene(vkBulletScene *scene);

  bool HasShape() const;

  void SetInertia(const vkVector3f &inertia);
  const vkVector3f &GetInertia() const;
  void UpdateInertia();

  void UpdateRigidBody();
  void UpdateTransform(const vkMatrix4f &transform);

  virtual void	getWorldTransform(btTransform& worldTrans) const;

  //Bullet only calls the update of worldtransform for active objects
  virtual void	setWorldTransform(const btTransform& worldTrans);

private:

  vkBulletBody *m_bulletBody;
  btCollisionShape *m_collisionShape;
  btCompoundShape *m_compoundShape;
  btRigidBody *m_rigidBody;
  vkVector3f m_inertia;

  std::vector<vkBulletShape*> m_shapes;
};

VK_CLASS()
class VKBULLET_API vkBulletBody : public IPhysicsBody
{
  friend class vkBulletBodyImpl;
  VK_CLASS_GEN_OBJECT;
public:
  vkBulletBody();
  virtual ~vkBulletBody();

  virtual void SetEntity(vkEntity *entity);
  virtual vkEntity *GetEntity();
  virtual const vkEntity *GetEntity() const;

  virtual void SetMode(vkPhysBodyMode mode);
  virtual vkPhysBodyMode GetMode() const;

  virtual void SetMass(float mass);
  virtual float GetMass() const;

  virtual void SetInertia(const vkVector3f &inertia);
  virtual const vkVector3f &GetInertia() const;

  virtual void UpdateInertia();

  const vkMatrix4f &GetMatrix() const;
  virtual vkTransformation GetTransform();
  virtual void FinishTransformation();

  virtual void AttachShape(IPhysicsShape *shape);
  virtual void DetachShape(IPhysicsShape *shape);

  void AttachToScene(vkBulletScene *scene);
  void DetachFromScene(vkBulletScene *scene);

private:
  vkBulletScene *m_scene;
  vkEntity *m_entity;
  vkPhysBodyMode m_bodyMode;
  float m_mass;
  vkMatrix4f m_transform;

  void DynamicallyChanged(const vkMatrix4f &transform);

  void UpdateBodies();
  std::vector<vkBulletBodyImpl*> m_bodies;


};

VK_FORCEINLINE const vkMatrix4f &vkBulletBody::GetMatrix() const
{
  return m_transform;
}


