#pragma once


#include <GraphicsGL4/Deferred/DefaultCollector.hh>

vkDefaultCollector::vkDefaultCollector(vkCollection<vkRenderState*> *renderState, vkCollection<vkLight*> *lights)
  : m_renderStates(renderState)
  , m_lights(lights)
{

}

void vkDefaultCollector::ScanGeometry(vkRenderState *renderState)
{
  if (m_renderStates)
  {
    m_renderStates->Add(renderState);
  }
}

void vkDefaultCollector::ScanLight(vkLight *light)
{
  if (m_lights)
  {
    m_lights->Add(light);
  }
}
