#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/graphics/vklight.hh>
#include <valkyrie/graphics/vkpointlight.refl.hh>

VK_CLASS()
class VKE_API vkPointLight : public VK_SUPER(vkLight)
{
  VK_CLASS_GEN;
public:
  vkPointLight();
  virtual ~vkPointLight();

  void SetPosition(const vkVector3f &position);
  const vkVector3f &GetPosition() const;

  void SetRadius(float radius);
  float GetRadius() const;

private:
  vkVector3f m_position;
  float m_radius;
};
