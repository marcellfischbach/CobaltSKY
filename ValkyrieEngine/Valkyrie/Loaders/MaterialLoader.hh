#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/Core/vkclass.hh>
#include <Valkyrie/Core/vkresourcemanager.hh>
#include <Valkyrie/Loaders/MaterialLoader.refl.hh>

/**
* \ingroup loading
*/
VK_CLASS()
class VKE_API vkMaterialAssetXMLLoader : public VK_SUPER(vkBaseXMLLoader)
{
  VK_CLASS_GEN;
public:
  vkMaterialAssetXMLLoader();
  virtual ~vkMaterialAssetXMLLoader();

  virtual bool CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;


};

