
#include <cobalt/graphics/csdirectionallight.hh>

csDirectionalLight::csDirectionalLight()
  : csLight(eLT_DirectionalLight)
  , m_direction(0.0f, 0.0f, 1.0f)
{
}

csDirectionalLight::~csDirectionalLight()
{

}

void csDirectionalLight::SetArbDirection(const csVector3f &arbDirection)
{
  arbDirection.Normalized(m_direction);
}

void csDirectionalLight::SetDirection(const csVector3f &direction)
{
  m_direction = direction;
}

const csVector3f &csDirectionalLight::GetDirection() const
{
  return m_direction;
}

