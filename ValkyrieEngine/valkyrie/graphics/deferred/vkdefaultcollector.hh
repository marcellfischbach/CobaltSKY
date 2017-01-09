#pragma once

#include <valkyrie/core/vkcollection.hh>
#include <valkyrie/entity/ientityscan.hh>

class vkRenderState;
class vkDefaultCollector : public iEntityScan
{
public:
  vkDefaultCollector(vkCollection<vkRenderState*> *renderStates,
                     vkCollection<vkLightState*> *lightStates);

  virtual void ScanRenderState(vkRenderState *renderState);

  virtual void ScanLightState(vkLightState *lightState);


private:
  vkCollection<vkRenderState*> *m_renderStates;
  vkCollection<vkLightState*> *m_lightStates;
};
