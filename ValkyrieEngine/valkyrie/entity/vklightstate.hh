#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/entity/vkspatialstate.hh>
#include <valkyrie/entity/vklightstate.refl.hh>

class vkLight;
/**
* \ingroup entity
*/

VK_CLASS()
class VKE_API vkLightState : public VK_SUPER(vkSpatialState)
{
  VK_CLASS_GEN;
public:
  vkLightState();
  virtual ~vkLightState();

  void SetLight(vkLight* light);
  vkLight *GetLight();
  const vkLight *GetLight() const;

  virtual void PrivScan(vkClipper *clipper, iGraphics *graphics, iEntityScan *entityScan, const vkScanConfig &config);
protected:
  virtual void FillBoundingBox(vkBoundingBox &bbox);

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
