#pragma once

class vkGeometryData;
class vkLight;

struct IEntityScan
{

  virtual void ScanGeometry(vkGeometryData *geometry) = 0;

  virtual void ScanLight(vkLight *light) = 0;

};