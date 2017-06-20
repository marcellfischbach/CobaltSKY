#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/entity/csspatialstate.hh>
#include <cobalt/entity/cslightstate.refl.hh>

class csLight;
/**
* \ingroup entity
*/

CS_CLASS()
class CSE_API csLightState : public CS_SUPER(csSpatialState)
{
  CS_CLASS_GEN;
public:
  csLightState();
  virtual ~csLightState();

  void SetLight(csLight* light);
  csLight *GetLight();
  const csLight *GetLight() const;

  virtual void PrivScan(csClipper *clipper, iGraphics *graphics, iEntityScan *entityScan, const csScanConfig &config);
protected:
  virtual void FillBoundingBox(csBoundingBox &bbox);

private:
  csLight *m_light;

};


CS_FORCEINLINE csLight *csLightState::GetLight()
{
  return m_light;
}

CS_FORCEINLINE const csLight *csLightState::GetLight() const
{
  return m_light;
}
