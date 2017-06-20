#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/graphics/cslight.hh>
#include <cobalt/graphics/cspointlight.refl.hh>

CS_CLASS()
class CSE_API csPointLight : public CS_SUPER(csLight)
{
  CS_CLASS_GEN;
public:
  csPointLight();
  virtual ~csPointLight();

  void SetPosition(const csVector3f &position);
  const csVector3f &GetPosition() const;

  void SetRadius(float radius);
  float GetRadius() const;

private:
  csVector3f m_position;
  float m_radius;
};
