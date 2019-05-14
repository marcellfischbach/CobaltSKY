#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/entity/csspatialstate.hh>
#include <cobalt/math/csvector.hh>
#include <cobalt/entity/csjointstate.refl.hh>


class csDynamicColliderState;

CS_CLASS()
class CSE_API csJointState : public CS_SUPER(csSpatialState)
{
  CS_CLASS_GEN;
public:
  virtual ~csJointState();

  enum TransformReference
  {
    eTR_ColliderA,
    eTR_ColliderB,
    eTR_Global,
  };

  void SetColliderA(csDynamicColliderState *state);
  csDynamicColliderState *GetColliderA();
  const csDynamicColliderState *GetColliderA() const;

  void SetColliderB(csDynamicColliderState *state);
  csDynamicColliderState *GetColliderB();
  const csDynamicColliderState *GetColliderB() const;

  void SetTransformReference(TransformReference reference);
  TransformReference GetTransformReference() const;


protected:
  csJointState();

  virtual void OnAssembled();

private:
  csDynamicColliderState *m_colliderA;
  csDynamicColliderState *m_colliderB;

  TransformReference m_transformReference;
};



CS_FORCEINLINE csDynamicColliderState *csJointState::GetColliderA()
{
  return m_colliderA;
}

CS_FORCEINLINE const csDynamicColliderState *csJointState::GetColliderA() const
{
  return m_colliderA;
}

CS_FORCEINLINE csDynamicColliderState *csJointState::GetColliderB()
{
  return m_colliderB;
}

CS_FORCEINLINE const csDynamicColliderState *csJointState::GetColliderB() const
{
  return m_colliderB;
}

CS_FORCEINLINE void csJointState::SetTransformReference(csJointState::TransformReference reference)
{
  m_transformReference = reference;
}

CS_FORCEINLINE csJointState::TransformReference csJointState::GetTransformReference() const
{
  return m_transformReference;
}
