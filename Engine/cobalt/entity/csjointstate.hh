#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/entity/csspatialstate.hh>
#include <cobalt/math/csvector.hh>
#include <cobalt/entity/csjointstate.refl.hh>

namespace cs
{
class DynamicColliderState;


CS_CLASS()
class CSE_API JointState : public CS_SUPER(cs::SpatialState)
{
  CS_CLASS_GEN;
public:
  virtual ~JointState();

  enum TransformReference
  {
    eTR_ColliderA,
    eTR_ColliderB,
    eTR_Global,
  };

  void SetColliderA(cs::DynamicColliderState * state);
  cs::DynamicColliderState* GetColliderA();
  const cs::DynamicColliderState* GetColliderA() const;

  void SetColliderB(cs::DynamicColliderState * state);
  cs::DynamicColliderState* GetColliderB();
  const cs::DynamicColliderState* GetColliderB() const;

  void SetTransformReference(TransformReference reference);
  TransformReference GetTransformReference() const;


protected:
  JointState();

  virtual void OnAssembled();

private:
  cs::DynamicColliderState* m_colliderA;
  cs::DynamicColliderState* m_colliderB;

  TransformReference m_transformReference;
};

}


CS_FORCEINLINE cs::DynamicColliderState *cs::JointState::GetColliderA()
{
  return m_colliderA;
}

CS_FORCEINLINE const cs::DynamicColliderState *cs::JointState::GetColliderA() const
{
  return m_colliderA;
}

CS_FORCEINLINE cs::DynamicColliderState *cs::JointState::GetColliderB()
{
  return m_colliderB;
}

CS_FORCEINLINE const cs::DynamicColliderState *cs::JointState::GetColliderB() const
{
  return m_colliderB;
}

CS_FORCEINLINE void cs::JointState::SetTransformReference(cs::JointState::TransformReference reference)
{
  m_transformReference = reference;
}

CS_FORCEINLINE cs::JointState::TransformReference cs::JointState::GetTransformReference() const
{
  return m_transformReference;
}
