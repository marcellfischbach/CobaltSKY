#pragma once
#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/core/vkresourcemanager.hh>
#include <Valkyrie/Loaders/entity/vkentitystatexmlloader.hh>
#include <Valkyrie/Loaders/entity/vkspatialstatexmlloader.refl.hh>

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
