

#include <valkyrie/entity/vkrenderstate.hh>
#include <valkyrie/graphics/igraphics.hh>


vkRenderState::vkRenderState()
  : vkSpatialState()
  , m_queue (eRQ_Deferred)
  , m_shadingMode(eSM_Shaded)
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

void vkRenderState::SetRenderQueue(vkRenderQueue queue)
{
  m_queue = queue;
}

vkRenderQueue vkRenderState::GetRenderQueue() const
{
  return m_queue;
}

void vkRenderState::SetShadingMode(vkShadingMode shadingMode)
{
  m_shadingMode = shadingMode;
}

vkShadingMode vkRenderState::GetShadingMode() const
{
  return m_shadingMode;
}

void vkRenderState::Render(iGraphics *graphics, vkRenderPass pass) const
{
  graphics->SetRenderFadeInOut(m_renderStartFade, m_renderEndFade);
  graphics->SetRenderFadeInOutValue(m_distanceState.GetFadeValue());
}
