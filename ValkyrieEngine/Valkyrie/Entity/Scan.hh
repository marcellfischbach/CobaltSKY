#pragma once

class vkGeometryMesh;
class vkLight;

struct IEntityScan
{

  virtual void ScanGeometry(vkGeometryMesh *geometry) = 0;

  virtual void ScanLight(vkLight *light) = 0;

};

struct vkScanConfig
{

  bool ScanShadowCasters;
  bool ScanNonShadowCasters;

};