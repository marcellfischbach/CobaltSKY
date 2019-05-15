#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/csenums.hh>
#include <csrefl/class.hh>
#include <cobalt/math/cscolor4f.hh>
#include <cobalt/math/csvector.hh>
#include <cobalt/graphics/cslight.refl.hh>



CS_CLASS()
class CSE_API csLight : public CS_SUPER(cs::Object)
{
  CS_CLASS_GEN;

public:
  csLight(csLightType type);
  virtual ~csLight();

  csLightType GetLightType() const;

  void SetColor(const csColor4f &color);
  const csColor4f &GetColor() const;

  void SetEnergy(float energy);
  float GetEnergy() const;

  void SetCastShadow(bool castShadow);
  bool IsCastingShadow() const;

  void SetShadowIntensity(float shadowIntensity);
  float GetShadowIntensity() const;

private:

  csLightType m_lightType;
  csColor4f m_color;
  float m_energy;

  bool m_castShadow;
  float m_shadowIntensity;

};





