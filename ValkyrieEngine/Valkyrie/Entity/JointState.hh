#pragma once


#include <Valkyrie/Export.hh>
#include <Valkyrie/Entity/SpatialState.hh>
#include <Valkyrie/Math/Vector.hh>
#include <Valkyrie/Entity/JointState.refl.hh>


class vkDynamicColliderState;
struct IPhysicsHingeJoint;

VK_INTERFACE()
class VKE_API vkJointState : public vkSpatialState
{
  VK_CLASS_GEN;
public:
  virtual ~vkJointState();

  enum TransformReference
  {
    eTR_ColliderA,
    eTR_ColliderB,
    eTR_Global,
  };

  void SetColliderA(vkDynamicColliderState *state);
  vkDynamicColliderState *GetColliderA();
  const vkDynamicColliderState *GetColliderA() const;

  void SetColliderB(vkDynamicColliderState *state);
  vkDynamicColliderState *GetColliderB();
  const vkDynamicColliderState *GetColliderB() const;

  void SetTransformReference(TransformReference reference);
  TransformReference GetTransformReference() const;


protected:
  vkJointState();

  virtual void OnAssembled();

private:
  vkDynamicColliderState *m_colliderA;
  vkDynamicColliderState *m_colliderB;

  TransformReference m_transformReference;
};


VK_CLASS()
class VKE_API vkHingeJointState : public vkJointState
{
  VK_CLASS_GEN;
public:
  vkHingeJointState();
  virtual ~vkHingeJointState();

protected:
  virtual void UpdateTransformation();

  virtual void OnAssembled();
  virtual void OnAttachedToScene(vkEntityScene *scene);
  virtual void OnDetachedFromScene(vkEntityScene *scene);

private:
  IPhysicsHingeJoint *m_hingeJoint;
};


VK_FORCEINLINE vkDynamicColliderState *vkJointState::GetColliderA()
{
  return m_colliderA;
}

VK_FORCEINLINE const vkDynamicColliderState *vkJointState::GetColliderA() const
{
  return m_colliderA;
}

VK_FORCEINLINE vkDynamicColliderState *vkJointState::GetColliderB()
{
  return m_colliderB;
}

VK_FORCEINLINE const vkDynamicColliderState *vkJointState::GetColliderB() const
{
  return m_colliderB;
}

VK_FORCEINLINE void vkJointState::SetTransformReference(vkJointState::TransformReference reference)
{
  m_transformReference = reference;
}

VK_FORCEINLINE vkJointState::TransformReference vkJointState::GetTransformReference() const
{
  return m_transformReference;
}
