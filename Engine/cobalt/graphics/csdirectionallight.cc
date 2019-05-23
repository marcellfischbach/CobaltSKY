
#include <cobalt/graphics/csdirectionallight.hh>

cs::DirectionalLight::DirectionalLight()
  : cs::Light(cs::eLT_DirectionalLight)
  , m_direction(0.0f, 0.0f, 1.0f)
{
}

cs::DirectionalLight::~DirectionalLight()
{

}

void cs::DirectionalLight::SetArbDirection(const cs::Vector3f &arbDirection)
{
  arbDirection.Normalized(m_direction);
}

void cs::DirectionalLight::SetDirection(const cs::Vector3f &direction)
{
  m_direction = direction;
}

const cs::Vector3f &cs::DirectionalLight::GetDirection() const
{
  return m_direction;
}

