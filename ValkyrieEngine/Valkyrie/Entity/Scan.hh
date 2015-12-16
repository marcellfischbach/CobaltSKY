#pragma once

class vkRenderState;
class vkLight;

struct IEntityScan
{

  virtual void ScanGeometry(vkRenderState *geometry) = 0;

  virtual void ScanLight(vkLight *light) = 0;

};

struct vkScanConfig
{

  bool ScanShadowCasters;
  bool ScanNonShadowCasters;

};