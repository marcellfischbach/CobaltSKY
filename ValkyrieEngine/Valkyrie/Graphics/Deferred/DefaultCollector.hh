#pragma once

#include <Valkyrie/Core/vkcollection.hh>
#include <Valkyrie/Entity/Scan.hh>

class vkRenderState;
class vkDefaultCollector : public IEntityScan
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
