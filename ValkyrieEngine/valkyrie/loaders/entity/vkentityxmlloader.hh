#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/core/vkresourcemanager.hh>
#include <valkyrie/loaders/entity/vkentityxmlloader.refl.hh>

VK_CLASS()
/**
* \ingroup loading
*/
class VKE_API vkEntityXMLLoader : public VK_SUPER(vkBaseXMLLoader)
{
  VK_CLASS_GEN;
public:
  vkEntityXMLLoader();
  virtual ~vkEntityXMLLoader();


  virtual bool CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;

  virtual const vkClass *GetLoadingClass() const;
};
