#pragma once


#include <Valkyrie/Export.hh>
#include <Valkyrie/Entity/EntityState.hh>

#include <Valkyrie/Entity/RigidBodyState.refl.hh>


VK_CLASS()
class VKE_API vkRigidBodyState : public vkEntityState
{
  VK_CLASS_GEN;
public:
  vkRigidBodyState();
  virtual ~vkRigidBodyState();

  void SetDynamic(bool dynamic);
  bool IsDynamic() const;

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
    bool m_dynamic;
  VK_PROPERTY()
    float m_mass;

  VK_PROPERTY()
    float m_friction;
  VK_PROPERTY()
    float m_restitution;

};


VK_FORCEINLINE void vkRigidBodyState::SetDynamic(bool dynamic)
{
  m_dynamic = dynamic;
}

VK_FORCEINLINE bool vkRigidBodyState::IsDynamic() const
{
  return m_dynamic;
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
