#pragma once
#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/core/vkresourcemanager.hh>
#include <Valkyrie/Loaders/entity/vkspatialstatexmlloader.hh>
#include <Valkyrie/Loaders/entity/vkcolliderstatexmlloader.refl.hh>


VK_CLASS()
class VKE_API vkColliderStateXMLLoader : public VK_SUPER(vkSpatialStateXMLLoader)
{
  VK_CLASS_GEN;
public:
  vkColliderStateXMLLoader();
  virtual ~vkColliderStateXMLLoader();

  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *GetLoadingClass() const;

};
