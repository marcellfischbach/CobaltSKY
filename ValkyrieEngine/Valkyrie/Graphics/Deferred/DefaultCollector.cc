#pragma once


#include <Valkyrie/Graphics/Deferred/DefaultCollector.hh>
#include <Valkyrie/entity/vkrenderstate.hh>

vkDefaultCollector::vkDefaultCollector(vkCollection<vkRenderState*> *renderStates,
                                       vkCollection<vkLightState*> *lightStates)
  : m_renderStates(renderStates)
  , m_lightStates(lightStates)
{

}

void vkDefaultCollector::ScanRenderState(vkRenderState *renderState)
{
  m_renderStates[renderState->GetRenderQueue()].Add(renderState);
}

void vkDefaultCollector::ScanLightState(vkLightState *lightState)
{
  if (m_lightStates)
  {
    m_lightStates->Add(lightState);
  }
}
