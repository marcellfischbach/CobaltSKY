#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/entity/csentitystate.hh>
#include <cobalt/entity/cstransformation.hh>

#include <vector>

#include <cobalt/entity/cstransformstate.refl.hh>

CS_CLASS()
class CSE_API csTransformState : public CS_SUPER(csEntityState)
{
  friend class csEntity;
  CS_CLASS_GEN;

public:
  csTransformState();

  csTransformation GetTransformation();
  virtual void FinishTransformation();

  void SetLocalTransformation(const csMatrix4f &localTransformation);
  const csMatrix4f &GetLocalTransformation() const;
  const csMatrix4f &GetGlobalTransformation() const;
  const csMatrix4f &GetGlobalTransformationInv() const;



private:
  CS_PROPERTY(LocalTransformation)
  csMatrix4f m_localMatrix;

  void PerformTransformation();
  void UpdateTransformation();

  bool m_dirty;
  mutable csMatrix4f m_globalMatrix;
  mutable csMatrix4f m_globalMatrixInv;

  csTransformState *m_parent;
  std::vector<csTransformState*> m_children;

};

CS_FORCEINLINE const csMatrix4f &csTransformState::GetLocalTransformation() const
{
  return m_localMatrix;
}