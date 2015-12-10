#pragma once


#include <Valkyrie/Export.hh>
#include <Valkyrie/Entity/SpatialState.hh>
#include <Valkyrie/Entity/GeometryState.refl.hh>

class vkGeometryBase;

VK_CLASS()
class VKE_API vkGeometryState : public vkSpatialState
{
  VK_CLASS_GEN;
public:
  vkGeometryState();
  virtual ~vkGeometryState();

  void SetGeometry(vkGeometryBase *geometry);
  vkGeometryBase *GetGeometry();
  const vkGeometryBase *GetGeometry() const;

  virtual void FinishTransformation();

protected:
  virtual void PrivScan(vkClipper *clipper, IGraphics *graphics, IEntityScan *entityScan);


private:
  vkGeometryBase *m_geometry;
};



VK_FORCEINLINE vkGeometryBase *vkGeometryState::GetGeometry()
{
  return m_geometry;
}

VK_FORCEINLINE const vkGeometryBase *vkGeometryState::GetGeometry() const
{
  return m_geometry;
}
