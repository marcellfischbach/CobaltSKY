#pragma once
#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/core/vkresourcemanager.hh>
#include <valkyrie/loaders/entity/vkspatialstatexmlloader.hh>
#include <valkyrie/loaders/entity/vkcolliderstatexmlloader.refl.hh>


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
