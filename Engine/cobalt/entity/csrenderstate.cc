

#include <cobalt/entity/csrenderstate.hh>
#include <cobalt/graphics/igraphics.hh>


cs::RenderState::RenderState()
  : cs::SpatialState()
  , m_queue (cs::eRQ_Deferred)
  , m_shadingMode(cs::eSM_Shaded)
  , m_renderStartFade(FLT_MAX)
  , m_renderEndFade(FLT_MAX)
{

}

cs::RenderState::~RenderState()
{

}

void cs::RenderState::SetFadeOut(float start, float end)
{
  m_renderStartFade = start;
  m_renderEndFade = end;
}

void cs::RenderState::SetRenderQueue(cs::eRenderQueue queue)
{
  m_queue = queue;
}

cs::eRenderQueue cs::RenderState::GetRenderQueue() const
{
  return m_queue;
}

void cs::RenderState::SetShadingMode(cs::eShadingMode shadingMode)
{
  m_shadingMode = shadingMode;
}

cs::eShadingMode cs::RenderState::GetShadingMode() const
{
  return m_shadingMode;
}

void cs::RenderState::Render(cs::iGraphics *graphics, cs::eRenderPass pass) const
{
  graphics->SetRenderFadeInOut(m_renderStartFade, m_renderEndFade);
  graphics->SetRenderFadeInOutValue(m_distanceState.GetFadeValue());
}
