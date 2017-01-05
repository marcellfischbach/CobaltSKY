
#include <Valkyrie/Graphics/vkpointlight.hh>

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


