

#include <graphicsgl4/deferred/gl4defaultcollector.hh>
#include <cobalt/entity/csrenderstate.hh>

cs::DefaultCollectorGL4::DefaultCollectorGL4(cs::Collection<cs::RenderState*> *renderStates,
                                       cs::Collection<cs::LightState*> *lightStates)
  : m_renderStates(renderStates)
  , m_lightStates(lightStates)
{

}

void cs::DefaultCollectorGL4::ScanRenderState(cs::RenderState *renderState)
{
  m_renderStates[renderState->GetRenderQueue()].Add(renderState);
}

void cs::DefaultCollectorGL4::ScanLightState(cs::LightState *lightState)
{
  if (m_lightStates)
  {
    m_lightStates->Add(lightState);
  }
}
