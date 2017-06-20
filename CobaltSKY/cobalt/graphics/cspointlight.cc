
#include <cobalt/graphics/cspointlight.hh>

csPointLight::csPointLight()
  : csLight(eLT_PointLight)
  , m_position(0.0f, 0.0f, 0.0f)
  , m_radius(100.0f)
{

}

csPointLight::~csPointLight()
{
}


void csPointLight::SetPosition(const csVector3f &position)
{
  m_position = position;
}

const csVector3f &csPointLight::GetPosition() const
{
  return m_position;
}


void csPointLight::SetRadius(float radius)
{
  m_radius = radius;
}

float csPointLight::GetRadius() const
{
  return m_radius;
}


