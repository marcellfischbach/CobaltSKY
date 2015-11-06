
#include <Valkyrie/Graphics/Light.hh>


vkLight::vkLight(vkLightType type)
  : vkObject()
  , m_lightType(type)
  , m_color(1.0f, 1.0f, 1.0f, 1.0f)
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





vkPointLight::vkPointLight()
  : vkLight(eLT_PointLight)
  , m_position(0.0f, 0.0f, 0.0f)
  , m_radius(100.0f)
{

}

vkPointLight::~vkPointLight()
{
}


void vkPointLight::SetPosition(const vkVector3f &position)
{
  m_position = position;
}

const vkVector3f &vkPointLight::GetPosition() const
{
  return m_position;
}


void vkPointLight::SetRadius(float radius)
{
  m_radius = radius;
}

float vkPointLight::GetRadius() const
{
  return m_radius;
}



vkDirectionalLight::vkDirectionalLight()
  : vkLight(eLT_DirectionalLight)
  , m_direction(0.0f, 0.0f, 1.0f)
{
}

vkDirectionalLight::~vkDirectionalLight()
{

}

void vkDirectionalLight::SetDirection(const vkVector3f &direction)
{
  m_direction = direction;
}

const vkVector3f &vkDirectionalLight::GetDirection() const
{
  return m_direction;
}

