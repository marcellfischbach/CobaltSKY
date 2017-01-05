#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/Graphics/Light.hh>

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

