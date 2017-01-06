
#include <valkyrie/entity/vkskinnedmeshstate.hh>
#include <valkyrie/animation/vkskeleton.hh>
#include <valkyrie/graphics/igraphics.hh>


vkSkinnedMeshState::vkSkinnedMeshState()
  : vkStaticMeshState()
  , m_skeleton(0)
{

}


vkSkinnedMeshState::~vkSkinnedMeshState()
{
  VK_RELEASE(m_skeleton);
}


void vkSkinnedMeshState::SetSkeleton(vkSkeleton *skeleton)
{
  VK_SET(m_skeleton, skeleton);
}


void vkSkinnedMeshState::Render(IGraphics *graphics, vkRenderPass pass) const
{
  if (m_skeleton)
  {
    graphics->SetSkeleton(m_skeleton);

    vkStaticMeshState::Render(graphics, pass);
  }
}
