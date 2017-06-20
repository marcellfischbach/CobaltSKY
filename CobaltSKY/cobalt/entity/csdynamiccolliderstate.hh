#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/entity/csbasecolliderstate.hh>

#include <cobalt/entity/csdynamiccolliderstate.refl.hh>

struct iPhysicsTriggerCollider;
struct iPhysicsDynamicCollider;
class csDynamicColliderStateTransformationCallback;

CS_CLASS()
class CSE_API csDynamicColliderState : public CS_SUPER(csBaseColliderState)
{
  CS_CLASS_GEN;
public:
  csDynamicColliderState();
  virtual ~csDynamicColliderState();

  iPhysicsDynamicCollider *GetDynamicCollider();
  const iPhysicsDynamicCollider *GetDynamicCollider() const;

  virtual void SetKinematic(bool kinematic);
  virtual bool IsKinematic() const;

  virtual void SetMass(float mass);
  virtual float GetMass() const;

  virtual void SetInertia(const csVector3f &inertia);
  virtual const csVector3f &GetInertia() const;

  virtual void SetAutoInertia(bool autoInertia);
  virtual bool IsAutoInertia() const;

  virtual void DynamicTransformationChanged(const csMatrix4f &transformation);

protected:
  virtual void OnAttachedToScene(csEntityScene *scene);
  virtual void OnDetachedFromScene(csEntityScene *scene);

private:
  iPhysicsDynamicCollider *m_dynamicCollider;
  csDynamicColliderStateTransformationCallback *m_callback;
};

CS_FORCEINLINE iPhysicsDynamicCollider *csDynamicColliderState::GetDynamicCollider()
{
  return m_dynamicCollider;
}

CS_FORCEINLINE const iPhysicsDynamicCollider *csDynamicColliderState::GetDynamicCollider() const
{
  return m_dynamicCollider;
}


