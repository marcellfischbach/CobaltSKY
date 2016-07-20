#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Core/ResourceManager.hh>
#include <Valkyrie/Loaders/MaterialLoader.refl.hh>

/**
* \ingroup loading
*/
VK_CLASS()
class VKE_API vkMaterialAssetXMLLoader : public vkBaseXMLLoader
{
  VK_CLASS_GEN;
public:
  vkMaterialAssetXMLLoader();
  virtual ~vkMaterialAssetXMLLoader();

  virtual bool CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;


};

