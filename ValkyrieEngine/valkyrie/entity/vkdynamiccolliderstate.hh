#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/entity/vkbasecolliderstate.hh>

#include <valkyrie/entity/vkdynamiccolliderstate.refl.hh>

struct IPhysicsTriggerCollider;
struct IPhysicsDynamicCollider;
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

VK_FORCEINLINE IPhysicsDynamicCollider *vkDynamicColliderState::GetDynamicCollider()
{
  return m_dynamicCollider;
}

VK_FORCEINLINE const IPhysicsDynamicCollider *vkDynamicColliderState::GetDynamicCollider() const
{
  return m_dynamicCollider;
}


