#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/graphics/vklight.hh>
#include <valkyrie/graphics/vkdirectionallight.refl.hh>

VK_CLASS()
class VKE_API vkDirectionalLight : public VK_SUPER(vkLight)
{
  VK_CLASS_GEN;
public:
  vkDirectionalLight();
  virtual ~vkDirectionalLight();

  void SetArbDirection(const vkVector3f &arbDirection);
  void SetDirection(const vkVector3f &direction);
  const vkVector3f &GetDirection() const;

private:
  vkVector3f m_direction;

};

