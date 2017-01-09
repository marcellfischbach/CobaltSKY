#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/resource/vkbasexmlloader.hh>
#include <valkyrie/core/resource/vkassetxmlloader.refl.hh>

VK_CLASS()
class VKE_API vkAssetXMLLoader : public VK_SUPER(vkBaseXMLLoader)
{
  VK_CLASS_GEN;
public:
  vkAssetXMLLoader();
  virtual ~vkAssetXMLLoader();

  virtual bool CanLoad(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData = 0) const;
  virtual const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData = 0) const;
  virtual iObject *Load(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData = 0) const;

};
