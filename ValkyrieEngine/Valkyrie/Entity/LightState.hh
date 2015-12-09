#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Entity/SpatialState.hh>
#include <Valkyrie/Entity/LightState.refl.hh>
class vkLight;

VK_CLASS()
class VKE_API vkLightState : public vkSpatialState
{
  VK_CLASS_GEN;
public:
  vkLightState();
  virtual ~vkLightState();

  void SetLight(vkLight* light);
  vkLight *GetLight();
  const vkLight *GetLight() const;

  virtual void PrivScan(vkClipper *clipper, IGraphics *graphics, IEntityScan *entityScan);

private:
  vkLight *m_light;

};


VK_FORCEINLINE vkLight *vkLightState::GetLight()
{
  return m_light;
}

VK_FORCEINLINE const vkLight *vkLightState::GetLight() const
{
  return m_light;
}
