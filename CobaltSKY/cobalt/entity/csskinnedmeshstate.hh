#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/entity/csstaticmeshstate.hh>
#include <cobalt/entity/csskinnedmeshstate.refl.hh>
class csSkeleton;

CS_CLASS()
class CSE_API csSkinnedMeshState : public CS_SUPER(csStaticMeshState)
{
  CS_CLASS_GEN;
public:
  csSkinnedMeshState();
  virtual ~csSkinnedMeshState();

  void SetSkeleton(csSkeleton *skeleton);
  csSkeleton *GetSkeleton();
  const csSkeleton *GetSkeleton() const;

  virtual void Render(iGraphics *graphics, csRenderPass pass) const;

private:
  csSkeleton *m_skeleton;
};


CS_FORCEINLINE csSkeleton *csSkinnedMeshState::GetSkeleton()
{
  return m_skeleton;
}

CS_FORCEINLINE const csSkeleton *csSkinnedMeshState::GetSkeleton() const
{
  return m_skeleton;
}
