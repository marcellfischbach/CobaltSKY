
#include <Valkyrie/Entity/GeometryState.hh>
#include <Valkyrie/Entity/Geometry.hh>
#include <Valkyrie/Entity/Scan.hh>

vkGeometryState::vkGeometryState()
  : vkSpatialState()
  , m_geometry(0)
  , m_castShadow(false)
{

}


vkGeometryState::~vkGeometryState()
{

}



void vkGeometryState::SetGeometry(vkGeometryData *geometry)
{
  if (geometry)
  {
    if (geometry->IsAttached())
    {
      // make a copy of the geometry prior to attaching
      m_geometry = geometry->Clone();
      m_geometry->SetAttached(true);
      return;
    }
  }

  VK_SET(m_geometry, geometry);
  if (m_geometry)
  {
    m_geometry->SetAttached(true);
  }
}



void vkGeometryState::PrivScan(vkClipper *clipper, IGraphics *graphics, IEntityScan *entityScan, const vkScanConfig &config)
{
  if (m_geometry)
  {
    if (config.ScanShadowCasters && m_castShadow || config.ScanNonShadowCasters && !m_castShadow)
    {
      m_geometry->Scan(clipper, graphics, entityScan, config);
    }
  }
}


void vkGeometryState::FinishTransformation()
{
  vkSpatialState::FinishTransformation();

  if (m_geometry)
  {
    m_geometry->UpdateTransformation(m_globalMatrix);
  }
}