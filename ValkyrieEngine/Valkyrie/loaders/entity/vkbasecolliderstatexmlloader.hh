#pragma once
#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/core/vkresourcemanager.hh>
#include <Valkyrie/loaders/entity/vkcolliderstatexmlloader.hh>
#include <Valkyrie/loaders/entity/vkbasecolliderstatexmlloader.refl.hh>

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
