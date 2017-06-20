

#include <cobalt/entity/csrenderstate.hh>
#include <cobalt/graphics/igraphics.hh>


csRenderState::csRenderState()
  : csSpatialState()
  , m_queue (eRQ_Deferred)
  , m_shadingMode(eSM_Shaded)
  , m_renderStartFade(FLT_MAX)
  , m_renderEndFade(FLT_MAX)
{

}

csRenderState::~csRenderState()
{

}

void csRenderState::SetFadeOut(float start, float end)
{
  m_renderStartFade = start;
  m_renderEndFade = end;
}

void csRenderState::SetRenderQueue(csRenderQueue queue)
{
  m_queue = queue;
}

csRenderQueue csRenderState::GetRenderQueue() const
{
  return m_queue;
}

void csRenderState::SetShadingMode(csShadingMode shadingMode)
{
  m_shadingMode = shadingMode;
}

csShadingMode csRenderState::GetShadingMode() const
{
  return m_shadingMode;
}

void csRenderState::Render(iGraphics *graphics, csRenderPass pass) const
{
  graphics->SetRenderFadeInOut(m_renderStartFade, m_renderEndFade);
  graphics->SetRenderFadeInOutValue(m_distanceState.GetFadeValue());
}
