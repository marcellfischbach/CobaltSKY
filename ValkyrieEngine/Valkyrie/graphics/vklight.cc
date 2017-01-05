
#include <Valkyrie/graphics/vklight.hh>


vkLight::vkLight(vkLightType type)
  : vkObject()
  , m_lightType(type)
  , m_color(1.0f, 1.0f, 1.0f, 1.0f)
  , m_energy(1.0f)
  , m_castShadow(false)
  , m_shadowIntensity(0.0f)
{

}

vkLight::~vkLight()
{

}

vkLightType vkLight::GetLightType() const
{
  return m_lightType;
}

void vkLight::SetColor(const vkColor4f &color)
{
  m_color = color;
}

const vkColor4f &vkLight::GetColor() const
{
  return m_color;
}

void vkLight::SetEnergy(float energy)
{
  m_energy = energy;
}

float vkLight::GetEnergy() const
{
  return m_energy;
}


void vkLight::SetCastShadow(bool castShadow)
{
  m_castShadow = castShadow;
}

bool vkLight::IsCastingShadow() const
{
  return m_castShadow;
}

void vkLight::SetShadowIntensity(float shadowIntensity)
{
  m_shadowIntensity = shadowIntensity;
}

float vkLight::GetShadowIntensity() const
{
  return m_shadowIntensity;
}


