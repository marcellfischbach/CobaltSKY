#pragma once


#include <RenderGL4/Deferred/DefaultCollector.hh>

vkDefaultCollector::vkDefaultCollector(vkCollection<vkGeometryNode*> *geometrieNodes, vkCollection<vkLightNode*> *lightNodes)
  : m_geometryNodes(geometrieNodes)
  , m_lightNodes(lightNodes)
{

}

bool vkDefaultCollector::ScanGeometryNode(vkGeometryNode *geometryNode)
{
  if (m_geometryNodes)
  {
    m_geometryNodes->Add(geometryNode);
  }
  return true;
}

bool vkDefaultCollector::ScanLightNode(vkLightNode *lightNode)
{
  if (m_lightNodes)
  {
    m_lightNodes->Add(lightNode);
  }
  return true;
}
