#pragma once


#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/ResourceManager.hh>
#include <Valkyrie/Loaders/MaterialInstanceLoader.refl.hh>

class vkMaterialInstance;


VK_CLASS()
class VKE_API vkMaterialInstanceAssetXMLLoader : public vkBaseXMLLoader
{
  VK_CLASS_GEN_OBJECT;
public:
  vkMaterialInstanceAssetXMLLoader();
  virtual ~vkMaterialInstanceAssetXMLLoader();

  virtual bool CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;

};
