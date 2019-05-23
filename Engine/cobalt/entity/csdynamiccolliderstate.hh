#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/entity/csbasecolliderstate.hh>

#include <cobalt/entity/csdynamiccolliderstate.refl.hh>




namespace cs
{
struct iPhysicsTriggerCollider;
struct iPhysicsDynamicCollider;
class DynamicColliderStateTransformationCallback;

CS_CLASS()
class CSE_API DynamicColliderState : public CS_SUPER(cs::BaseColliderState)
{
  CS_CLASS_GEN;
public:
  DynamicColliderState();
  virtual ~DynamicColliderState();

  cs::iPhysicsDynamicCollider* GetDynamicCollider();
  const cs::iPhysicsDynamicCollider* GetDynamicCollider() const;

  CS_FUNCTION()
    virtual void SetKinematic(bool kinematic);

  CS_FUNCTION()
    virtual bool IsKinematic() const;

  CS_FUNCTION()
    virtual void SetMass(float mass);
  CS_FUNCTION()
    virtual float GetMass() const;

  CS_FUNCTION()
    virtual void SetInertia(const cs::Vector3f & inertia);
  CS_FUNCTION()
    virtual const cs::Vector3f& GetInertia() const;

  CS_FUNCTION()
    virtual void SetAutoInertia(bool autoInertia);
  CS_FUNCTION()
    virtual bool IsAutoInertia() const;

  virtual void DynamicTransformationChanged(const cs::Matrix4f & transformation);

protected:
  virtual void OnAttachedToScene(cs::EntityScene * scene);
  virtual void OnDetachedFromScene(cs::EntityScene * scene);

private:
  CS_PROPERTY()
    float m_mass;

  CS_PROPERTY()
    cs::Vector3f m_inertia;

  CS_PROPERTY()
    bool m_autoInertia;


  cs::iPhysicsDynamicCollider* m_dynamicCollider;
  cs::DynamicColliderStateTransformationCallback* m_callback;
};

}

CS_FORCEINLINE cs::iPhysicsDynamicCollider* cs::DynamicColliderState::GetDynamicCollider()
{
  return m_dynamicCollider;
}

CS_FORCEINLINE const cs::iPhysicsDynamicCollider* cs::DynamicColliderState::GetDynamicCollider() const
{
  return m_dynamicCollider;
}


