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

  void SetCastShadow(bool castShadow);
  bool IsCastShadow() const;

  virtual void FinishTransformation();

protected:
  virtual void PrivScan(vkClipper *clipper, IGraphics *graphics, IEntityScan *entityScan, const vkScanConfig &config);


private:
  vkGeometryData *m_geometry;

  bool m_castShadow;
};



VK_FORCEINLINE vkGeometryData *vkGeometryState::GetGeometry()
{
  return m_geometry;
}

VK_FORCEINLINE const vkGeometryData *vkGeometryState::GetGeometry() const
{
  return m_geometry;
}

VK_FORCEINLINE void vkGeometryState::SetCastShadow(bool castShadow)
{
  m_castShadow = castShadow;
}

VK_FORCEINLINE bool vkGeometryState::IsCastShadow() const
{
  return m_castShadow;
}