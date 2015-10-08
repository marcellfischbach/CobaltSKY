#pragma once


class vkGeometryNode;

struct vkScanCallback
{

  virtual bool ScanGeometryNode(vkGeometryNode *geometryNode) = 0;

};