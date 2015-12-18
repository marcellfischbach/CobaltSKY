#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Core/ResourceManager.hh>
#include <Valkyrie/Loaders/MaterialLoader.refl.hh>

/**
* \ingroup loading
*/
VK_CLASS();
class VKE_API vkMaterialLoader : public vkBaseXMLLoader
{
  VK_CLASS_GEN;
public:
  vkMaterialLoader();
  virtual ~vkMaterialLoader();

  virtual bool CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;


};


/**
* \ingroup loading
*/
VK_CLASS();
class VKE_API vkMaterialInstanceLoader : public vkBaseXMLLoader
{
  VK_CLASS_GEN;
public:
  vkMaterialInstanceLoader();
  virtual ~vkMaterialInstanceLoader();

  virtual bool CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;


};