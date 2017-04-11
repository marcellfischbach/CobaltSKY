#pragma once

#include <valkyrie/core/vkclass.hh>
#include <valkyrie/core/vkresourcemanager.hh>
#include <loaders/loaderseditoriconassetxmlloader.refl.hh>

VK_CLASS()
class LoadersEditorIconAssetXMLLoader : public VK_SUPER(vkBaseXMLLoader)
{
  VK_CLASS_GEN;
public:
  LoadersEditorIconAssetXMLLoader();
  virtual ~LoadersEditorIconAssetXMLLoader();

  virtual bool CanLoad(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData = 0) const;
  virtual const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData = 0) const;
  virtual iObject *Load(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData = 0) const;


};