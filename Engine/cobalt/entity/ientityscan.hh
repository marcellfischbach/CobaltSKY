#pragma once

#include <cobalt/math/csvector.hh>

namespace cs
{
class LightState;
class RenderState;

/**
* \ingroup entity
*/

struct iEntityScan
{

  virtual void ScanRenderState(cs::RenderState *renderState) = 0;

  virtual void ScanLightState(cs::LightState *lightState) = 0;

};

/**
* \ingroup entity
*/

struct ScanConfig
{
  ScanConfig()
    : FrameNo(0)
  {
  }

  csUInt64 FrameNo;

  bool ScanShadowCasters;
  bool ScanNonShadowCasters;

  cs::Vector3f MainCameraPosition;
};

}
