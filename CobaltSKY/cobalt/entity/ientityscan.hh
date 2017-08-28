#pragma once

#include <cobalt/math/csvector.hh>

class csRenderState;
class csLightState;

/**
* \ingroup entity
*/

struct iEntityScan
{

  virtual void ScanRenderState(csRenderState *renderState) = 0;

  virtual void ScanLightState(csLightState *lightState) = 0;

};

/**
* \ingroup entity
*/

struct csScanConfig
{
  csScanConfig()
    : FrameNo(0)
  {
  }

  csUInt64 FrameNo;

  bool ScanShadowCasters;
  bool ScanNonShadowCasters;

  csVector3f MainCameraPosition;
};