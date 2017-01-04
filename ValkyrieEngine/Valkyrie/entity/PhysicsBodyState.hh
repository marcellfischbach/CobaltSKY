#pragma once


#include <Valkyrie/Export.hh>
#include <Valkyrie/Entity/EntityState.hh>
#include <Valkyrie/Entity/SpatialState.hh>
#include <Valkyrie/Enums.hh>
#include <Valkyrie/Physics/PhysicsShapeContainer.hh>

#include <Valkyrie/Entity/PhysicsBodyState.refl.hh>


VK_CLASS()
class VKE_API vkCollisionState : public vkSpatialState
{
  VK_CLASS_GEN;
public:
  vkCollisionState();
  virtual ~vkCollisionState();

  void SetFriction(float friction);
  float GetFriction() const;

  void SetRestitution(float restitution);
  float GetRestitution() const;

  void SetShape(vkPhysicsShapeContainer *shape);
  vkPhysicsShapeContainer *GetShape() const;

public:
  
  VK_PROPERTY()
    float m_friction;
  VK_PROPERTY()
    float m_restitution;

  VK_PROPERTY()
    vkPhysicsShapeContainer *m_shape;
};

VK_CLASS()
class VKE_API vkStaticCollisionState : public vkCollisionState
{
  VK_CLASS_GEN;
public:
  vkStaticCollisionState();
  ~vkStaticCollisionState();

};

VK_CLASS()
class VKE_API vkDynamicCollisionState : public vkCollisionState
{
  VK_CLASS_GEN;
public:
  vkDynamicCollisionState();
  virtual ~vkDynamicCollisionState();

  void SetKinematic(bool kinematic);
  bool IsKinematic() const;

  void SetMass(float mass);
  float GetMass() const;

  void UpdateBody();

  virtual void OnAttachedToEntity(vkEntity *entity);

public:

  VK_PROPERTY()
    bool m_kinematic;

  VK_PROPERTY()
    float m_mass;

};


VK_FORCEINLINE void vkDynamicBodyState::SetMode(vkPhysBodyMode mode)
{
  m_mode = mode;
}

VK_FORCEINLINE vkPhysBodyMode vkDynamicBodyState::GetMode() const
{
  return m_mode;
}

VK_FORCEINLINE void vkDynamicBodyState::SetMass(float mass)
{
  m_mass = mass;
}

VK_FORCEINLINE float vkDynamicBodyState::GetMass() const
{
  return m_mass;
}

VK_FORCEINLINE void vkDynamicBodyState::SetFriction(float friction)
{
  m_friction = friction;
}

VK_FORCEINLINE float vkDynamicBodyState::GetFriction() const
{
  return m_friction;
}

VK_FORCEINLINE void vkDynamicBodyState::SetRestitution(float restitution)
{
  m_restitution = restitution;
}

VK_FORCEINLINE float vkRigidBodyState::GetRestitution() const
{
  return m_restitution;
}
