
#include <cobalt/entity/csskinnedmeshstate.hh>
#include <cobalt/animation/csskeleton.hh>
#include <cobalt/graphics/igraphics.hh>


cs::SkinnedMeshState::SkinnedMeshState()
  : cs::StaticMeshState()
  , m_skeleton(0)
{

}


cs::SkinnedMeshState::~SkinnedMeshState()
{
  CS_RELEASE(m_skeleton);
}


void cs::SkinnedMeshState::SetSkeleton(cs::Skeleton *skeleton)
{
  CS_SET(m_skeleton, skeleton);
}


void cs::SkinnedMeshState::Render(cs::iGraphics *graphics, cs::eRenderPass pass) const
{
  if (m_skeleton)
  {
    graphics->SetSkeleton(m_skeleton);

    cs::StaticMeshState::Render(graphics, pass);
  }
}
