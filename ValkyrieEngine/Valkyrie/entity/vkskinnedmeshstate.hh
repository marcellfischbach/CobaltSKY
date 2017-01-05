#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/entity/vkstaticmeshstate.hh>
#include <Valkyrie/entity/vkskinnedmeshstate.refl.hh>
class vkSkeleton;

VK_CLASS()
class VKE_API vkSkinnedMeshState : public VK_SUPER(vkStaticMeshState)
{
  VK_CLASS_GEN;
public:
  vkSkinnedMeshState();
  virtual ~vkSkinnedMeshState();

  void SetSkeleton(vkSkeleton *skeleton);
  vkSkeleton *GetSkeleton();
  const vkSkeleton *GetSkeleton() const;

  virtual void Render(IGraphics *graphics, vkRenderPass pass) const;

private:
  vkSkeleton *m_skeleton;
};


VK_FORCEINLINE vkSkeleton *vkSkinnedMeshState::GetSkeleton()
{
  return m_skeleton;
}

VK_FORCEINLINE const vkSkeleton *vkSkinnedMeshState::GetSkeleton() const
{
  return m_skeleton;
}
