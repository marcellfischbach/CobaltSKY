#pragma once


class vkGeometryNode;
class vkLightNode;

struct IScanCallback
{

  virtual bool ScanGeometryNode(vkGeometryNode *geometryNode) = 0;

  virtual bool ScanLightNode(vkLightNode *lightNode) = 0;

};

