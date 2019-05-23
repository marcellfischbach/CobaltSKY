

#include <graphicsgl4/deferred/gl4defaultcollector.hh>
#include <cobalt/entity/csrenderstate.hh>

csDefaultCollectorGL4::csDefaultCollectorGL4(cs::Collection<cs::RenderState*> *renderStates,
                                       cs::Collection<cs::LightState*> *lightStates)
  : m_renderStates(renderStates)
  , m_lightStates(lightStates)
{

}

void csDefaultCollectorGL4::ScanRenderState(cs::RenderState *renderState)
{
  m_renderStates[renderState->GetRenderQueue()].Add(renderState);
}

void csDefaultCollectorGL4::ScanLightState(cs::LightState *lightState)
{
  if (m_lightStates)
  {
    m_lightStates->Add(lightState);
  }
}
