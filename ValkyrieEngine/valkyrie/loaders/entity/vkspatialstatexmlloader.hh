#pragma once
#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/core/vkresourcemanager.hh>
#include <valkyrie/loaders/entity/vkentitystatexmlloader.hh>
#include <valkyrie/loaders/entity/vkspatialstatexmlloader.refl.hh>

VK_CLASS()
/**
* \ingroup loading
*/
class VKE_API vkSpatialStateXMLLoader : public VK_SUPER(vkEntityStateXMLLoader)
{
  VK_CLASS_GEN;
public:
  vkSpatialStateXMLLoader();
  virtual ~vkSpatialStateXMLLoader();

  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *GetLoadingClass() const;

};
