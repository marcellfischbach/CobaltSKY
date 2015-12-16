#pragma once


#include <GraphicsGL4/Deferred/DefaultCollector.hh>

vkDefaultCollector::vkDefaultCollector(vkCollection<vkRenderState*> *renderStates, vkCollection<vkLightState*> *lightStates)
  : m_renderStates(renderStates)
  , m_lightStates(lightStates)
{

}

void vkDefaultCollector::ScanRenderState(vkRenderState *renderState)
{
  if (m_renderStates)
  {
    m_renderStates->Add(renderState);
  }
}

void vkDefaultCollector::ScanLightState(vkLightState *lightState)
{
  if (m_lightStates)
  {
    m_lightStates->Add(lightState);
  }
}
