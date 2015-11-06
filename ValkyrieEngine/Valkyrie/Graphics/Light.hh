#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Enums.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Math/Color.hh>
#include <Valkyrie/Math/Vector.hh>
#include <Valkyrie/Graphics/Light.refl.hh>



VK_INTERFACE()
class VKE_API vkLight : public vkObject
{
  VK_CLASS_GEN;

public:
  vkLight(vkLightType type);
  virtual ~vkLight();

  vkLightType GetLightType() const;

  void SetColor(const vkColor4f &color);
  const vkColor4f &GetColor() const;


private:

  vkLightType m_lightType;
  vkColor4f m_color;


};


VK_CLASS()
class VKE_API vkPointLight : public vkLight
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


VK_CLASS()
class VKE_API vkDirectionalLight : public vkLight
{
  VK_CLASS_GEN;
public:
  vkDirectionalLight();
  virtual ~vkDirectionalLight();

  void SetDirection(const vkVector3f &direction);
  const vkVector3f &GetDirection() const;

private:
  vkVector3f m_direction;

};

