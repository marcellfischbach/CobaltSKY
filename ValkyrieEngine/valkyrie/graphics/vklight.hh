#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/vkenums.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/math/vkcolor4f.hh>
#include <valkyrie/math/vkvector.hh>
#include <valkyrie/graphics/vklight.refl.hh>


VK_INTERFACE()
class VKE_API vkLight : public VK_SUPER(vkObject)
{
  VK_CLASS_GEN;

public:
  vkLight(vkLightType type);
  virtual ~vkLight();

  vkLightType GetLightType() const;

  void SetColor(const vkColor4f &color);
  const vkColor4f &GetColor() const;

  void SetEnergy(float energy);
  float GetEnergy() const;

  void SetCastShadow(bool castShadow);
  bool IsCastingShadow() const;

  void SetShadowIntensity(float shadowIntensity);
  float GetShadowIntensity() const;

private:

  vkLightType m_lightType;
  vkColor4f m_color;
  float m_energy;

  bool m_castShadow;
  float m_shadowIntensity;

};





