#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/resource/vkbasexmlloader.hh>
#include <Valkyrie/core/resource/vkassetxmlloader.refl.hh>

VK_CLASS()
class VKE_API vkAssetXMLLoader : public VK_SUPER(vkBaseXMLLoader)
{
  VK_CLASS_GEN;
public:
  vkAssetXMLLoader();
  virtual ~vkAssetXMLLoader();

  virtual bool CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;

};
