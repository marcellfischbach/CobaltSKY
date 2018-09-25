#pragma once


class csGeometryNode;
class csLightNode;

struct iScanCallback
{

  virtual bool ScanGeometryNode(csGeometryNode *geometryNode) = 0;

  virtual bool ScanLightNode(csLightNode *lightNode) = 0;

};

