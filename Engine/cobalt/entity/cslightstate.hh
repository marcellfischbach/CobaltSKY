#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/entity/csspatialstate.hh>
#include <cobalt/entity/cslightstate.refl.hh>

/**
* \ingroup entity
*/

namespace cs
{
class Light;

CS_CLASS()
class CSE_API LightState : public CS_SUPER(cs::SpatialState)
{
  CS_CLASS_GEN;
public:
  LightState();
  virtual ~LightState();

  void SetLight(cs::Light * light);
  cs::Light* GetLight();
  const cs::Light* GetLight() const;

  virtual void PrivScan(cs::Clipper * clipper, cs::iGraphics * graphics, cs::iEntityScan * entityScan, const cs::ScanConfig & config);
protected:
  virtual void FillBoundingBox(cs::BoundingBox & bbox);

private:
  cs::Light* m_light;

};

}


CS_FORCEINLINE cs::Light *cs::LightState::GetLight()
{
  return m_light;
}

CS_FORCEINLINE const cs::Light *cs::LightState::GetLight() const
{
  return m_light;
}
