#pragma once


class vkGeometryNode;
class vkLightNode;

struct iScanCallback
{

  virtual bool ScanGeometryNode(vkGeometryNode *geometryNode) = 0;

  virtual bool ScanLightNode(vkLightNode *lightNode) = 0;

};

