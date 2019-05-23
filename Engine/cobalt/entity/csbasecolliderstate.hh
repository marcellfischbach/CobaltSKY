#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/entity/cscolliderstate.hh>

#include <cobalt/entity/csbasecolliderstate.refl.hh>


namespace cs
{
struct iPhysicsBaseCollider;

CS_CLASS()
class CSE_API BaseColliderState : public CS_SUPER(cs::ColliderState)
{
  CS_CLASS_GEN;
public:
  virtual ~BaseColliderState();

  void SetFriction(float friction);
  float GetFriction() const;

  void SetRestitution(float restitution);
  float GetRestitution() const;

  cs::iPhysicsBaseCollider* GetBaseCollider();
  const cs::iPhysicsBaseCollider* GetBaseCollider() const;

protected:
  BaseColliderState();

  void SetBaseCollider(cs::iPhysicsBaseCollider * baseCollider);

private:

  cs::iPhysicsBaseCollider* m_baseCollider;
};
}


CS_FORCEINLINE cs::iPhysicsBaseCollider* cs::BaseColliderState::GetBaseCollider()
{
  return m_baseCollider;
}

CS_FORCEINLINE const cs::iPhysicsBaseCollider* cs::BaseColliderState::GetBaseCollider() const
{
  return m_baseCollider;
}


