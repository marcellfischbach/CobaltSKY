

#include <graphicsgl4/deferred/gl4defaultcollector.hh>
#include <cobalt/entity/csrenderstate.hh>

csDefaultCollectorGL4::csDefaultCollectorGL4(csCollection<csRenderState*> *renderStates,
                                       csCollection<csLightState*> *lightStates)
  : m_renderStates(renderStates)
  , m_lightStates(lightStates)
{

}

void csDefaultCollectorGL4::ScanRenderState(csRenderState *renderState)
{
  m_renderStates[renderState->GetRenderQueue()].Add(renderState);
}

void csDefaultCollectorGL4::ScanLightState(csLightState *lightState)
{
  if (m_lightStates)
  {
    m_lightStates->Add(lightState);
  }
}
