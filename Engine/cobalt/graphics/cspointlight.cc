
#include <cobalt/graphics/cspointlight.hh>

cs::PointLight::PointLight()
  : cs::Light(cs::eLT_PointLight)
  , m_position(0.0f, 0.0f, 0.0f)
  , m_radius(100.0f)
{

}

cs::PointLight::~PointLight()
{
}


void cs::PointLight::SetPosition(const cs::Vector3f &position)
{
  m_position = position;
}

const cs::Vector3f &cs::PointLight::GetPosition() const
{
  return m_position;
}


void cs::PointLight::SetRadius(float radius)
{
  m_radius = radius;
}

float cs::PointLight::GetRadius() const
{
  return m_radius;
}


