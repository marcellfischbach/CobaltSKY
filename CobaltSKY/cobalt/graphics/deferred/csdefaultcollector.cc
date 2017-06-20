

#include <cobalt/graphics/deferred/csdefaultcollector.hh>
#include <cobalt/entity/csrenderstate.hh>

csDefaultCollector::csDefaultCollector(csCollection<csRenderState*> *renderStates,
                                       csCollection<csLightState*> *lightStates)
  : m_renderStates(renderStates)
  , m_lightStates(lightStates)
{

}

void csDefaultCollector::ScanRenderState(csRenderState *renderState)
{
  m_renderStates[renderState->GetRenderQueue()].Add(renderState);
}

void csDefaultCollector::ScanLightState(csLightState *lightState)
{
  if (m_lightStates)
  {
    m_lightStates->Add(lightState);
  }
}
