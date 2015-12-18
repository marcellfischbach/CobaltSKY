#pragma once

class vkRenderState;
class vkLightState;

/**
* \ingroup entity
*/

struct IEntityScan
{

  virtual void ScanRenderState(vkRenderState *renderState) = 0;

  virtual void ScanLightState(vkLightState *lightState) = 0;

};

/**
* \ingroup entity
*/

struct vkScanConfig
{

  bool ScanShadowCasters;
  bool ScanNonShadowCasters;

};