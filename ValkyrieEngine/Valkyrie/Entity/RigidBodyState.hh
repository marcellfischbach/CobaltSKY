#pragma once


#include <Valkyrie/Export.hh>
#include <Valkyrie/Entity/EntityState.hh>
#include <Valkyrie/Enums.hh>

#include <Valkyrie/Entity/RigidBodyState.refl.hh>


VK_CLASS()
class VKE_API vkRigidBodyState : public vkEntityState
{
  VK_CLASS_GEN;
public:
  vkRigidBodyState();
  virtual ~vkRigidBodyState();

  void SetMode(vkPhysBodyMode mode);
  vkPhysBodyMode GetMode() const;

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
    vkPhysBodyMode m_mode;
  VK_PROPERTY()
    float m_mass;

  VK_PROPERTY()
    float m_friction;
  VK_PROPERTY()
    float m_restitution;

};


VK_FORCEINLINE void vkRigidBodyState::SetMode(vkPhysBodyMode mode)
{
  m_mode = mode;
}

VK_FORCEINLINE vkPhysBodyMode vkRigidBodyState::GetMode() const
{
  return m_mode;
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