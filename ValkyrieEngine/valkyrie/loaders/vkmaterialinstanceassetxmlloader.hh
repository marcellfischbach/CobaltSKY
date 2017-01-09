#pragma once


#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkresourcemanager.hh>
#include <valkyrie/loaders/vkmaterialinstanceassetxmlloader.refl.hh>

class vkMaterialInstance;


VK_CLASS()
class VKE_API vkMaterialInstanceAssetXMLLoader : public VK_SUPER(vkBaseXMLLoader)
{
  VK_CLASS_GEN_OBJECT;
public:
  vkMaterialInstanceAssetXMLLoader();
  virtual ~vkMaterialInstanceAssetXMLLoader();

  virtual bool CanLoad(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData = 0) const;
  virtual const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData = 0) const;
  virtual iObject *Load(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData = 0) const;

};
