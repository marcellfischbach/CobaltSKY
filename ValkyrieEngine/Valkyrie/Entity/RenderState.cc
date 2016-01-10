

#include <Valkyrie/Entity/RenderState.hh>
#include <Valkyrie/Graphics/IGraphics.hh>
vkRenderState::vkRenderState()
  : vkSpatialState()
  , m_renderStartFade(FLT_MAX)
  , m_renderEndFade(FLT_MAX)
{

}

vkRenderState::~vkRenderState()
{

}

void vkRenderState::SetFadeOut(float start, float end)
{
  m_renderStartFade = start;
  m_renderEndFade = end;
}

void vkRenderState::Render(IGraphics *graphics, vkRenderPass pass) const
{
  graphics->SetRenderFadeInOut(m_renderStartFade, m_renderEndFade);
}
