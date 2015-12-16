#pragma once

class vkRenderState;
class vkLightState;

struct IEntityScan
{

  virtual void ScanRenderState(vkRenderState *renderState) = 0;

  virtual void ScanLightState(vkLightState *lightState) = 0;

};

struct vkScanConfig
{

  bool ScanShadowCasters;
  bool ScanNonShadowCasters;

};