
#include <cobalt/graphics/cslight.hh>


csLight::csLight(csLightType type)
  : csObject()
  , m_lightType(type)
  , m_color(1.0f, 1.0f, 1.0f, 1.0f)
  , m_energy(1.0f)
  , m_castShadow(false)
  , m_shadowIntensity(0.0f)
{

}

csLight::~csLight()
{

}

csLightType csLight::GetLightType() const
{
  return m_lightType;
}

void csLight::SetColor(const csColor4f &color)
{
  m_color = color;
}

const csColor4f &csLight::GetColor() const
{
  return m_color;
}

void csLight::SetEnergy(float energy)
{
  m_energy = energy;
}

float csLight::GetEnergy() const
{
  return m_energy;
}


void csLight::SetCastShadow(bool castShadow)
{
  m_castShadow = castShadow;
}

bool csLight::IsCastingShadow() const
{
  return m_castShadow;
}

void csLight::SetShadowIntensity(float shadowIntensity)
{
  m_shadowIntensity = shadowIntensity;
}

float csLight::GetShadowIntensity() const
{
  return m_shadowIntensity;
}


