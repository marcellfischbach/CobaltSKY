#pragma once


#include <Valkyrie/vkexport.hh>
#include <Valkyrie/Entity/EntityState.hh>
#include <Valkyrie/Entity/SpatialState.hh>
#include <Valkyrie/vkenums.hh>
#include <Valkyrie/Entity/ColliderState.refl.hh>


struct IPhysicsShape;
struct IPhysicsCollider;
struct IPhysicsTriggerCollider;
struct IPhysicsBaseCollider;
struct IPhysicsStaticCollider;
struct IPhysicsDynamicCollider;
class vkPhysicsShapeContainer;


VK_INTERFACE()
class VKE_API vkColliderState : public VK_SUPER(vkSpatialState)
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


protected:
  vkColliderState();

  virtual void UpdateTransformation();

  void SetCollider(IPhysicsCollider *collider);

  bool m_updateTransformationGuard;

private:
  IPhysicsCollider *m_collider;

};


VK_INTERFACE()
class VKE_API vkBaseColliderState : public VK_SUPER(vkColliderState)
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
class VKE_API vkStaticColliderState : public VK_SUPER(vkBaseColliderState)
{
VK_CLASS_GEN;
public:
  vkStaticColliderState();
  virtual ~vkStaticColliderState ();

  IPhysicsStaticCollider *GetStaticCollider ();
  const IPhysicsStaticCollider *GetStaticCollider () const;

protected:
  virtual void OnAttachedToScene(vkEntityScene *scene);
  virtual void OnDetachedFromScene(vkEntityScene *scene);

private:
  IPhysicsStaticCollider *m_staticCollider;

};


class vkDynamicColliderStateTransformationCallback;

VK_CLASS()
class VKE_API vkDynamicColliderState : public VK_SUPER(vkBaseColliderState)
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

protected:
  virtual void OnAttachedToScene(vkEntityScene *scene);
  virtual void OnDetachedFromScene(vkEntityScene *scene);

private:
  IPhysicsDynamicCollider *m_dynamicCollider;
  vkDynamicColliderStateTransformationCallback *m_callback;
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

