#pragma once

#include <Valkyrie/Core/Collection.hh>
#include <Valkyrie/Entity/Scan.hh>

class vkRenderState;
class vkDefaultCollector : public IEntityScan
{
public:
  vkDefaultCollector(vkCollection<vkRenderState*> *renderStatesGBuffer,
                     vkCollection<vkRenderState*> *renderStatesForward,
                     vkCollection<vkRenderState*> *renderStatesForwardTransparent,
                     vkCollection<vkLightState*> *lightStates);

  virtual void ScanRenderState(vkRenderState *renderState);

  virtual void ScanLightState(vkLightState *lightState);


private:
  vkCollection<vkRenderState*> *m_renderStatesDeferred;
  vkCollection<vkRenderState*> *m_renderStatesForward;
  vkCollection<vkRenderState*> *m_renderStatesForwardTransprent;
  vkCollection<vkLightState*> *m_lightStates;
};
