
#include <cobalt/graphics/cslight.hh>


cs::Light::Light(cs::eLightType type)
  : cs::Object()
  , m_lightType(type)
  , m_color(1.0f, 1.0f, 1.0f, 1.0f)
  , m_energy(1.0f)
  , m_castShadow(false)
  , m_shadowIntensity(0.0f)
{

}

cs::Light::~Light()
{

}

cs::eLightType cs::Light::GetLightType() const
{
  return m_lightType;
}

void cs::Light::SetColor(const cs::Color4f &color)
{
  m_color = color;
}

const cs::Color4f &cs::Light::GetColor() const
{
  return m_color;
}

void cs::Light::SetEnergy(float energy)
{
  m_energy = energy;
}

float cs::Light::GetEnergy() const
{
  return m_energy;
}


void cs::Light::SetCastShadow(bool castShadow)
{
  m_castShadow = castShadow;
}

bool cs::Light::IsCastingShadow() const
{
  return m_castShadow;
}

void cs::Light::SetShadowIntensity(float shadowIntensity)
{
  m_shadowIntensity = shadowIntensity;
}

float cs::Light::GetShadowIntensity() const
{
  return m_shadowIntensity;
}


