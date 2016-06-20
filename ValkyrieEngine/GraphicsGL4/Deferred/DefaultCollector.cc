#pragma once


#include <GraphicsGL4/Deferred/DefaultCollector.hh>
#include <Valkyrie/Entity/RenderState.hh>

vkDefaultCollector::vkDefaultCollector(vkCollection<vkRenderState*> *renderStatesDeferred,
                                       vkCollection<vkRenderState*> *renderStatesForward,
                                       vkCollection<vkRenderState*> *renderStatesForwardTransparent,
                                       vkCollection<vkRenderState*> *renderStatesParticle,
                                       vkCollection<vkLightState*> *lightStates)
  : m_renderStatesDeferred(renderStatesDeferred)
  , m_renderStatesForward(renderStatesForward)
  , m_renderStatesForwardTransprent(renderStatesForwardTransparent)
  , m_renderStatesParticle(renderStatesParticle)
  , m_lightStates(lightStates)
{

}

void vkDefaultCollector::ScanRenderState(vkRenderState *renderState)
{
  switch(renderState->GetRenderQueue())
  {
  case eRQ_Deferred:
    if (m_renderStatesDeferred)
    {
      m_renderStatesDeferred->Add(renderState);
    }
    break;

  case eRQ_Forward:
    if (m_renderStatesForward)
    {
      m_renderStatesForward->Add(renderState);
    }
    break;

  case eRQ_Particles:
    if (m_renderStatesParticle)
    {
      m_renderStatesParticle->Add(renderState);
    }
    break;

  case eRQ_ForwardTransparent:
    if (m_renderStatesForwardTransprent)
    {
      m_renderStatesForwardTransprent->Add(renderState);
    }
    break;

  }

}

void vkDefaultCollector::ScanLightState(vkLightState *lightState)
{
  if (m_lightStates)
  {
    m_lightStates->Add(lightState);
  }
}
