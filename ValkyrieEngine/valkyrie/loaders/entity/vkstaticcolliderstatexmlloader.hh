#pragma once
#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/core/vkresourcemanager.hh>
#include <valkyrie/loaders/entity/vkbasecolliderstatexmlloader.hh>
#include <valkyrie/loaders/entity/vkstaticcolliderstatexmlloader.refl.hh>

VK_CLASS()
class VKE_API vkStaticColliderStateXMLLoader : public VK_SUPER(vkBaseColliderStateXMLLoader)
{
  VK_CLASS_GEN;
public:
  vkStaticColliderStateXMLLoader();
  virtual ~vkStaticColliderStateXMLLoader();

  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *GetLoadingClass() const;

};
