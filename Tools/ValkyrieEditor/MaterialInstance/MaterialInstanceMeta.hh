#pragma once


#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Core/ResourceManager.hh>
#include <map>
#include <MaterialInstance/MaterialInstanceMeta.refl.hh>

VK_CLASS()
class MaterialInstanceMeta : public vkObject
{
  VK_CLASS_GEN;
public:
  MaterialInstanceMeta();
  virtual ~MaterialInstanceMeta();


  void SetShaderResourceLocator(const vkResourceLocator &shaderLocator);
  const vkResourceLocator& GetShaderResourceLocator() const;

  void RemoveResourceLocator(const vkString &resourceName);
  bool HasResourceLocator(const vkString &resourceName) const;
  void SetResourceLocator(const vkString &resourceName, const vkResourceLocator &locator);
  vkResourceLocator GetResourceLocator(const vkString &resourceName) const;

private:

  vkResourceLocator m_shaderResourceLocator;

  std::map<vkString, vkResourceLocator> m_resourceLocators;

};
