#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/entity/csstaticmeshstate.hh>
#include <cobalt/entity/csskinnedmeshstate.refl.hh>

namespace cs
{
class Skeleton;

CS_CLASS()
class CSE_API SkinnedMeshState : public CS_SUPER(cs::StaticMeshState)
{
  CS_CLASS_GEN;
public:
  SkinnedMeshState();
  virtual ~SkinnedMeshState();

  void SetSkeleton(cs::Skeleton *skeleton);
  cs::Skeleton *GetSkeleton();
  const cs::Skeleton *GetSkeleton() const;

  virtual void Render(cs::iGraphics *graphics, cs::eRenderPass pass) const;

private:
  cs::Skeleton *m_skeleton;
};


}

CS_FORCEINLINE cs::Skeleton *cs::SkinnedMeshState::GetSkeleton()
{
  return m_skeleton;
}

CS_FORCEINLINE const cs::Skeleton *cs::SkinnedMeshState::GetSkeleton() const
{
  return m_skeleton;
}
