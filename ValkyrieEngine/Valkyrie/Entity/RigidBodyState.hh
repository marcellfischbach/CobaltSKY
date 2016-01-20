#pragma once


#include <Valkyrie/Export.hh>
#include <Valkyrie/Entity/EntityState.hh>
#include <Valkyrie/Entity/SpatialState.hh>
#include <Valkyrie/Enums.hh>

#include <Valkyrie/Entity/RigidBodyState.refl.hh>

struct IPhysicsShape;
struct IPhysicsCollider;
struct IPhysicsTriggerCollider;
struct IPhysicsBaseCollider;
struct IPhysicsStaticCollider;
struct IPhysicsDynamicCollider;
class vkPhysicsShapeContainer;

VK_CLASS()
class VKE_API vkRigidBodyState : public vkEntityState
{
  VK_CLASS_GEN;
public:
  vkRigidBodyState();
  virtual ~vkRigidBodyState();

  void SetMode(vkPhysBodyMode mode);
  vkPhysBodyMode GetMode() const;

  void SetMass(float mass);
  float GetMass() const;

  void SetFriction(float friction);
  float GetFriction() const;

  void SetRestitution(float restitution);
  float GetRestitution() const;

  void UpdateBody();

  virtual void OnAttachedToEntity(vkEntity *entity);

public:

  VK_PROPERTY()
    vkPhysBodyMode m_mode;
  VK_PROPERTY()
    float m_mass;

  VK_PROPERTY()
    float m_friction;
  VK_PROPERTY()
    float m_restitution;

};


VK_INTERFACE()
class VKE_API vkColliderState : public vkSpatialState
{
  VK_CLASS_GEN;
public:
  virtual ~vkColliderState();


  void AttachShape(IPhysicsShape *shape);
  void DetachShape(IPhysicsShape *shape);

  void AttachShape(vkPhysicsShapeContainer *shapes);
  void DetachShape(vkPhysicsShapeContainer *shapes);

  IPhysicsCollider *GetCollider();
  const IPhysicsCollider *GetCollider() const;

  virtual void FinishTransformation();

protected:
  vkColliderState();

  void SetCollider(IPhysicsCollider *collider);

private:
  bool m_updateTransformationGuard;
  IPhysicsCollider *m_collider;

};


VK_INTERFACE()
class VKE_API vkBaseColliderState : public vkColliderState
{
  VK_CLASS_GEN;
public:
  virtual ~vkBaseColliderState();

  void SetFriction(float friction);
  float GetFriction() const;

  void SetRestitution(float restitution);
  float GetRestitution() const;

  IPhysicsBaseCollider *GetBaseCollider();
  const IPhysicsBaseCollider *GetBaseCollider() const;

protected:
  vkBaseColliderState();

  void SetBaseCollider(IPhysicsBaseCollider *baseCollider);

private:

  IPhysicsBaseCollider *m_baseCollider;
};

VK_CLASS()
class VKE_API vkStaticColliderState : public vkBaseColliderState
{
VK_CLASS_GEN;
public:
  vkStaticColliderState();
  virtual ~vkStaticColliderState ();

  IPhysicsStaticCollider *GetStaticCollider ();
  const IPhysicsStaticCollider *GetStaticCollider () const;

  virtual void OnAttachedToScene(vkEntityScene *scene);
  virtual void OnDetachedFromScene(vkEntityScene *scene);

private:
  IPhysicsStaticCollider *m_staticCollider;

};


VK_CLASS()
class VKE_API vkDynamicColliderState : public vkBaseColliderState
{
  VK_CLASS_GEN;
public:
  vkDynamicColliderState();
  virtual ~vkDynamicColliderState();

  IPhysicsDynamicCollider *GetDynamicCollider();
  const IPhysicsDynamicCollider *GetDynamicCollider() const;

  virtual void SetKinematic(bool kinematic);
  virtual bool IsKinematic() const;

  virtual void SetMass(float mass);
  virtual float GetMass() const;

  virtual void SetInertia(const vkVector3f &inertia);
  virtual const vkVector3f &GetInertia() const;

  virtual void SetAutoInertia(bool autoInertia);
  virtual bool IsAutoInertia() const;

  virtual void DynamicTransformationChanged(const vkMatrix4f &transformation);

  virtual void OnAttachedToScene(vkEntityScene *scene);
  virtual void OnDetachedFromScene(vkEntityScene *scene);

private:
  IPhysicsDynamicCollider *m_dynamicCollider;
};

VK_FORCEINLINE IPhysicsCollider *vkColliderState::GetCollider()
{
  return m_collider;
}

VK_FORCEINLINE const IPhysicsCollider *vkColliderState::GetCollider() const
{
  return m_collider;
}

VK_FORCEINLINE IPhysicsBaseCollider *vkBaseColliderState::GetBaseCollider()
{
  return m_baseCollider;
}

VK_FORCEINLINE const IPhysicsBaseCollider *vkBaseColliderState::GetBaseCollider() const
{
  return m_baseCollider;
}

VK_FORCEINLINE IPhysicsStaticCollider *vkStaticColliderState::GetStaticCollider()
{
  return m_staticCollider;
}

VK_FORCEINLINE const IPhysicsStaticCollider *vkStaticColliderState::GetStaticCollider() const
{
  return m_staticCollider;
}

VK_FORCEINLINE IPhysicsDynamicCollider *vkDynamicColliderState::GetDynamicCollider()
{
  return m_dynamicCollider;
}

VK_FORCEINLINE const IPhysicsDynamicCollider *vkDynamicColliderState::GetDynamicCollider() const
{
  return m_dynamicCollider;
}



VK_FORCEINLINE void vkRigidBodyState::SetMode(vkPhysBodyMode mode)
{
  m_mode = mode;
}

VK_FORCEINLINE vkPhysBodyMode vkRigidBodyState::GetMode() const
{
  return m_mode;
}

VK_FORCEINLINE void vkRigidBodyState::SetMass(float mass)
{
  m_mass = mass;
}

VK_FORCEINLINE float vkRigidBodyState::GetMass() const
{
  return m_mass;
}

VK_FORCEINLINE void vkRigidBodyState::SetFriction(float friction)
{
  m_friction = friction;
}

VK_FORCEINLINE float vkRigidBodyState::GetFriction() const
{
  return m_friction;
}

VK_FORCEINLINE void vkRigidBodyState::SetRestitution(float restitution)
{
  m_restitution = restitution;
}

VK_FORCEINLINE float vkRigidBodyState::GetRestitution() const
{
  return m_restitution;
}
