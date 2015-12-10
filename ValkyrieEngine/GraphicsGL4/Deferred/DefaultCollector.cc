#pragma once


#include <GraphicsGL4/Deferred/DefaultCollector.hh>

vkDefaultCollector::vkDefaultCollector(vkCollection<vkGeometryData*> *geometrieDatas, vkCollection<vkLight*> *lights)
  : m_geometryDatas(geometrieDatas)
  , m_lights(lights)
{

}

void vkDefaultCollector::ScanGeometry(vkGeometryData *geometry)
{
  if (m_geometryDatas)
  {
    m_geometryDatas->Add(geometry);
  }
}

void vkDefaultCollector::ScanLight(vkLight *light)
{
  if (m_lights)
  {
    m_lights->Add(light);
  }
}
