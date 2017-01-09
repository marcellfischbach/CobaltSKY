#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/entity/vkcolliderstate.hh>

#include <valkyrie/entity/vkbasecolliderstate.refl.hh>

struct iPhysicsBaseCollider;

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

  iPhysicsBaseCollider *GetBaseCollider();
  const iPhysicsBaseCollider *GetBaseCollider() const;

protected:
  vkBaseColliderState();

  void SetBaseCollider(iPhysicsBaseCollider *baseCollider);

private:

  iPhysicsBaseCollider *m_baseCollider;
};


VK_FORCEINLINE iPhysicsBaseCollider *vkBaseColliderState::GetBaseCollider()
{
  return m_baseCollider;
}

VK_FORCEINLINE const iPhysicsBaseCollider *vkBaseColliderState::GetBaseCollider() const
{
  return m_baseCollider;
}

