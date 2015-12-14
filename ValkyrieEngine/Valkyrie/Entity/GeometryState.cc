
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



void vkGeometryState::SetGeometry(vkGeometryData *geometry)
{
  if (geometry)
  {
    if (geometry->IsAttached())
    {
      // make a copy of the geometry prior to attaching
      m_geometry = geometry->Clone();
      return;
    }
  }

  VK_SET(m_geometry, geometry);
}



void vkGeometryState::PrivScan(vkClipper *clipper, IGraphics *graphics, IEntityScan *entityScan)
{
  if (m_geometry)
  {
    m_geometry->Scan(clipper, graphics, entityScan);
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