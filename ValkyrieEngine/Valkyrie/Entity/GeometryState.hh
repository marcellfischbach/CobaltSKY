#pragma once


#include <Valkyrie/Export.hh>
#include <Valkyrie/Entity/SpatialState.hh>
#include <Valkyrie/Entity/GeometryState.refl.hh>

class vkGeometryData;

VK_CLASS()
class VKE_API vkGeometryState : public vkSpatialState
{
  VK_CLASS_GEN;
public:
  vkGeometryState();
  virtual ~vkGeometryState();

  void SetGeometry(vkGeometryData *geometry);
  vkGeometryData *GetGeometry();
  const vkGeometryData *GetGeometry() const;

  virtual void FinishTransformation();

protected:
  virtual void PrivScan(vkClipper *clipper, IGraphics *graphics, IEntityScan *entityScan);


private:
  vkGeometryData *m_geometry;
};



VK_FORCEINLINE vkGeometryData *vkGeometryState::GetGeometry()
{
  return m_geometry;
}

VK_FORCEINLINE const vkGeometryData *vkGeometryState::GetGeometry() const
{
  return m_geometry;
}
