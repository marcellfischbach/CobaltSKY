#pragma once


#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkresourcemanager.hh>
#include <Valkyrie/Loaders/vkmaterialinstanceassetxmlloader.refl.hh>

class vkMaterialInstance;


VK_CLASS()
class VKE_API vkMaterialInstanceAssetXMLLoader : public VK_SUPER(vkBaseXMLLoader)
{
  VK_CLASS_GEN_OBJECT;
public:
  vkMaterialInstanceAssetXMLLoader();
  virtual ~vkMaterialInstanceAssetXMLLoader();

  virtual bool CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;

};
