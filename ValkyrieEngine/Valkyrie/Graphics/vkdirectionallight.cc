
#include <Valkyrie/Graphics/vkdirectionallight.hh>

vkDirectionalLight::vkDirectionalLight()
  : vkLight(eLT_DirectionalLight)
  , m_direction(0.0f, 0.0f, 1.0f)
{
}

vkDirectionalLight::~vkDirectionalLight()
{

}

void vkDirectionalLight::SetArbDirection(const vkVector3f &arbDirection)
{
  arbDirection.Normalized(m_direction);
}

void vkDirectionalLight::SetDirection(const vkVector3f &direction)
{
  m_direction = direction;
}

const vkVector3f &vkDirectionalLight::GetDirection() const
{
  return m_direction;
}

