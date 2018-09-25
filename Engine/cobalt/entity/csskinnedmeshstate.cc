
#include <cobalt/entity/csskinnedmeshstate.hh>
#include <cobalt/animation/csskeleton.hh>
#include <cobalt/graphics/igraphics.hh>


csSkinnedMeshState::csSkinnedMeshState()
  : csStaticMeshState()
  , m_skeleton(0)
{

}


csSkinnedMeshState::~csSkinnedMeshState()
{
  CS_RELEASE(m_skeleton);
}


void csSkinnedMeshState::SetSkeleton(csSkeleton *skeleton)
{
  CS_SET(m_skeleton, skeleton);
}


void csSkinnedMeshState::Render(iGraphics *graphics, csRenderPass pass) const
{
  if (m_skeleton)
  {
    graphics->SetSkeleton(m_skeleton);

    csStaticMeshState::Render(graphics, pass);
  }
}
