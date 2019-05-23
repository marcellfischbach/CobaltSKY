#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/entity/csentitystate.hh>
#include <cobalt/entity/cstransformation.hh>

#include <vector>

#include <cobalt/entity/cstransformstate.refl.hh>

namespace cs
{

CS_CLASS()
class CSE_API TransformState : public CS_SUPER(cs::EntityState)
{
  friend class cs::Entity;
  CS_CLASS_GEN;

public:
  TransformState();

  cs::Transformation GetTransformation();
  virtual void FinishTransformation();

  void SetLocalTransformation(const cs::Matrix4f & localTransformation);
  const cs::Matrix4f& GetLocalTransformation() const;
  const cs::Matrix4f& GetGlobalTransformation() const;
  const cs::Matrix4f& GetGlobalTransformationInv() const;



private:
  CS_PROPERTY(name = LocalTransformation)
    cs::Matrix4f m_localMatrix;

  void PerformTransformation();
  void UpdateTransformation();

  bool m_dirty;
  mutable cs::Matrix4f m_globalMatrix;
  mutable cs::Matrix4f m_globalMatrixInv;

  cs::TransformState * m_parent;
  std::vector<cs::TransformState*> m_children;

};

}

CS_FORCEINLINE const cs::Matrix4f &cs::TransformState::GetLocalTransformation() const
{
  return m_localMatrix;
}
