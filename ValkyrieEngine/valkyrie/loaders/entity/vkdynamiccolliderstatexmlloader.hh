#pragma once
#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/core/vkresourcemanager.hh>
#include <valkyrie/loaders/entity/vkbasecolliderstatexmlloader.hh>
#include <valkyrie/loaders/entity/vkdynamiccolliderstatexmlloader.refl.hh>

VK_CLASS()
class VKE_API vkDynamicColliderStateXMLLoader : public VK_SUPER(vkBaseColliderStateXMLLoader)
{
  VK_CLASS_GEN;
public:
  vkDynamicColliderStateXMLLoader();
  virtual ~vkDynamicColliderStateXMLLoader();

  virtual iObject *Load(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData = 0) const;
  virtual const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData = 0) const;
  virtual const vkClass *GetLoadingClass() const;

};

