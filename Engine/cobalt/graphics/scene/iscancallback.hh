#pragma once


namespace cs
{
class GeometryNode;
class LightNode;

struct iScanCallback
{

  virtual bool ScanGeometryNode(cs::GeometryNode* geometryNode) = 0;

  virtual bool ScanLightNode(cs::LightNode* lightNode) = 0;

};


}
