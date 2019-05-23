#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/csenums.hh>
#include <csrefl/class.hh>
#include <cobalt/math/cscolor4f.hh>
#include <cobalt/math/csvector.hh>
#include <cobalt/graphics/cslight.refl.hh>

namespace cs
{

CS_CLASS()
class CSE_API Light : public CS_SUPER(cs::Object)
{
  CS_CLASS_GEN;

public:
  Light(cs::eLightType type);
  virtual ~Light();

  cs::eLightType GetLightType() const;

  void SetColor(const cs::Color4f & color);
  const cs::Color4f& GetColor() const;

  void SetEnergy(float energy);
  float GetEnergy() const;

  void SetCastShadow(bool castShadow);
  bool IsCastingShadow() const;

  void SetShadowIntensity(float shadowIntensity);
  float GetShadowIntensity() const;

private:

  cs::eLightType m_lightType;
  cs::Color4f m_color;
  float m_energy;

  bool m_castShadow;
  float m_shadowIntensity;

};


}
