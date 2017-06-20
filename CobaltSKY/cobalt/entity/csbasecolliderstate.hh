#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/entity/cscolliderstate.hh>

#include <cobalt/entity/csbasecolliderstate.refl.hh>

struct iPhysicsBaseCollider;

CS_INTERFACE()
class CSE_API csBaseColliderState : public CS_SUPER(csColliderState)
{
  CS_CLASS_GEN;
public:
  virtual ~csBaseColliderState();

  void SetFriction(float friction);
  float GetFriction() const;

  void SetRestitution(float restitution);
  float GetRestitution() const;

  iPhysicsBaseCollider *GetBaseCollider();
  const iPhysicsBaseCollider *GetBaseCollider() const;

protected:
  csBaseColliderState();

  void SetBaseCollider(iPhysicsBaseCollider *baseCollider);

private:

  iPhysicsBaseCollider *m_baseCollider;
};


CS_FORCEINLINE iPhysicsBaseCollider *csBaseColliderState::GetBaseCollider()
{
  return m_baseCollider;
}

CS_FORCEINLINE const iPhysicsBaseCollider *csBaseColliderState::GetBaseCollider() const
{
  return m_baseCollider;
}

