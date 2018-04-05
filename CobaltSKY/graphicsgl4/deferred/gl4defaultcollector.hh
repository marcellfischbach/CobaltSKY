#pragma once

#include <graphicsgl4/gl4export.hh>
#include <cobalt/core/cscollection.hh>
#include <cobalt/entity/ientityscan.hh>

class csRenderState;
class csDefaultCollectorGL4 : public iEntityScan
{
public:
  csDefaultCollectorGL4(csCollection<csRenderState*> *renderStates,
                     csCollection<csLightState*> *lightStates);

  virtual void ScanRenderState(csRenderState *renderState);

  virtual void ScanLightState(csLightState *lightState);


private:
  csCollection<csRenderState*> *m_renderStates;
  csCollection<csLightState*> *m_lightStates;
};
