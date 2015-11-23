#pragma once

#include <Valkyrie/Graphics/Scene/Scan.hh>
#include <Valkyrie/Core/Collection.hh>

class vkDefaultCollector : public IScanCallback
{
public:
  vkDefaultCollector(vkCollection<vkGeometryNode*> *geometrieNodes, vkCollection<vkLightNode*> *lightNodes);

  virtual bool ScanGeometryNode(vkGeometryNode *geometryNode);
  virtual bool ScanLightNode(vkLightNode *lightNode);

private:
  vkCollection<vkGeometryNode*> *m_geometryNodes;
  vkCollection<vkLightNode*> *m_lightNodes;
};
