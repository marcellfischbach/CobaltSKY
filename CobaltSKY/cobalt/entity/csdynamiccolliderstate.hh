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

  CS_FUNCTION()
    virtual void SetKinematic(bool kinematic);

  CS_FUNCTION()
    virtual bool IsKinematic() const;

  CS_FUNCTION()
    virtual void SetMass(float mass);
  CS_FUNCTION()
    virtual float GetMass() const;

  CS_FUNCTION()
    virtual void SetInertia(const csVector3f &inertia);
  CS_FUNCTION()
    virtual const csVector3f &GetInertia() const;

  CS_FUNCTION()
    virtual void SetAutoInertia(bool autoInertia);
  CS_FUNCTION()
    virtual bool IsAutoInertia() const;

  virtual void DynamicTransformationChanged(const csMatrix4f &transformation);

protected:
  virtual void OnAttachedToScene(csEntityScene *scene);
  virtual void OnDetachedFromScene(csEntityScene *scene);

private:
  CS_PROPERTY(name = Mass)
    float m_mass;

  CS_PROPERTY(name = Inertia)
    csVector3f m_inertia;

  CS_PROPERTY(name = AutoInertia)
    bool m_autoInertia;


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


