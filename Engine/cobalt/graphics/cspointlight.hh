#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/cslight.hh>
#include <cobalt/graphics/cspointlight.refl.hh>

namespace cs
{

CS_CLASS()
class CSE_API PointLight : public CS_SUPER(cs::Light)
{
  CS_CLASS_GEN;
public:
  PointLight();
  virtual ~PointLight();

  void SetPosition(const cs::Vector3f & position);
  const cs::Vector3f& GetPosition() const;

  void SetRadius(float radius);
  float GetRadius() const;

private:
  cs::Vector3f m_position;
  float m_radius;
};

}
