#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/cslight.hh>
#include <cobalt/graphics/csdirectionallight.refl.hh>

CS_CLASS()
class CSE_API csDirectionalLight : public CS_SUPER(csLight)
{
  CS_CLASS_GEN;
public:
  csDirectionalLight();
  virtual ~csDirectionalLight();

  void SetArbDirection(const csVector3f &arbDirection);
  void SetDirection(const csVector3f &direction);
  const csVector3f &GetDirection() const;

private:
  csVector3f m_direction;

};

