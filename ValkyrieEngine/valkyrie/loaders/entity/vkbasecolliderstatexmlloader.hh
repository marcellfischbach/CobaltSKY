#pragma once
#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/core/vkresourcemanager.hh>
#include <valkyrie/loaders/entity/vkcolliderstatexmlloader.hh>
#include <valkyrie/loaders/entity/vkbasecolliderstatexmlloader.refl.hh>

VK_CLASS()
class VKE_API vkBaseColliderStateXMLLoader : public VK_SUPER(vkColliderStateXMLLoader)
{
  VK_CLASS_GEN;
public:
  vkBaseColliderStateXMLLoader();
  virtual ~vkBaseColliderStateXMLLoader();

  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *GetLoadingClass() const;

};
