#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/cslight.hh>
#include <cobalt/graphics/csdirectionallight.refl.hh>

namespace cs
{

CS_CLASS()
class CSE_API DirectionalLight : public CS_SUPER(cs::Light)
{
  CS_CLASS_GEN;
public:
  DirectionalLight();
  virtual ~DirectionalLight();

  void SetArbDirection(const cs::Vector3f & arbDirection);
  void SetDirection(const cs::Vector3f & direction);
  const cs::Vector3f& GetDirection() const;

private:
  cs::Vector3f m_direction;

};


}

