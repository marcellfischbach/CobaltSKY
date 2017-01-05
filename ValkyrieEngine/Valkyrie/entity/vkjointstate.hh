#pragma once


#include <Valkyrie/vkexport.hh>
#include <Valkyrie/entity/vkspatialstate.hh>
#include <Valkyrie/Math/vkvector.hh>
#include <Valkyrie/entity/vkjointstate.refl.hh>


class vkDynamicColliderState;

VK_INTERFACE()
class VKE_API vkJointState : public VK_SUPER(vkSpatialState)
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
