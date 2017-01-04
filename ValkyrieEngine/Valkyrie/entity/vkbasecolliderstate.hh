#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/entity/vkcolliderstate.hh>

#include <Valkyrie/entity/vkbasecolliderstate.refl.hh>

struct IPhysicsBaseCollider;

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


VK_FORCEINLINE IPhysicsBaseCollider *vkBaseColliderState::GetBaseCollider()
{
  return m_baseCollider;
}

VK_FORCEINLINE const IPhysicsBaseCollider *vkBaseColliderState::GetBaseCollider() const
{
  return m_baseCollider;
}

