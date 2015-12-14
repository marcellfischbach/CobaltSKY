#pragma once


#include <GraphicsGL4/Deferred/DefaultCollector.hh>

vkDefaultCollector::vkDefaultCollector(vkCollection<vkGeometryMesh*> *geometrieDatas, vkCollection<vkLight*> *lights)
  : m_geometryMeshes(geometrieDatas)
  , m_lights(lights)
{

}

void vkDefaultCollector::ScanGeometry(vkGeometryMesh *geometry)
{
  if (m_geometryMeshes)
  {
    m_geometryMeshes->Add(geometry);
  }
}

void vkDefaultCollector::ScanLight(vkLight *light)
{
  if (m_lights)
  {
    m_lights->Add(light);
  }
}
