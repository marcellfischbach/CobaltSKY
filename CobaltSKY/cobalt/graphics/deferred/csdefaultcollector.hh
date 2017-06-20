#pragma once

#include <cobalt/core/cscollection.hh>
#include <cobalt/entity/ientityscan.hh>

class csRenderState;
class csDefaultCollector : public iEntityScan
{
public:
  csDefaultCollector(csCollection<csRenderState*> *renderStates,
                     csCollection<csLightState*> *lightStates);

  virtual void ScanRenderState(csRenderState *renderState);

  virtual void ScanLightState(csLightState *lightState);


private:
  csCollection<csRenderState*> *m_renderStates;
  csCollection<csLightState*> *m_lightStates;
};
