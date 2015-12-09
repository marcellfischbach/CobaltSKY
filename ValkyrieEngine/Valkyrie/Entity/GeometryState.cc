
#include <Valkyrie/Entity/GeometryState.hh>
#include <Valkyrie/Entity/Geometry.hh>

vkGeometryState::vkGeometryState()
  : vkSpatialState()
  , m_geometry(0)
{

}


vkGeometryState::~vkGeometryState()
{

}



void vkGeometryState::SetGeometry(vkGeometryBase *geometry)
{
  VK_SET(m_geometry, geometry);
}



void vkGeometryState::PrivScan(vkClipper *clipper, IGraphics *graphics, IEntityScan *entityScan)
{
  if (m_geometry)
  {
    m_geometry->Scan(clipper, graphics, entityScan);
  }
}