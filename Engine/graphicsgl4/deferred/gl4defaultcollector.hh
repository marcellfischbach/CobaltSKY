#pragma once

#include <graphicsgl4/gl4export.hh>
#include <cobalt/core/cscollection.hh>
#include <cobalt/entity/ientityscan.hh>

namespace cs
{
class RenderState;


class DefaultCollectorGL4 : public cs::iEntityScan
{
public:
  DefaultCollectorGL4(cs::Collection<cs::RenderState*> *renderStates,
                     cs::Collection<cs::LightState*> *lightStates);

  virtual void ScanRenderState(cs::RenderState *renderState);

  virtual void ScanLightState(cs::LightState *lightState);


private:
  cs::Collection<cs::RenderState*> *m_renderStates;
  cs::Collection<cs::LightState*> *m_lightStates;
};

}
