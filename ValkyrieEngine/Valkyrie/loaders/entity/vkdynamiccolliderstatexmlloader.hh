#pragma once
#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/core/vkresourcemanager.hh>
#include <Valkyrie/loaders/entity/vkbasecolliderstatexmlloader.hh>
#include <Valkyrie/loaders/entity/vkdynamiccolliderstatexmlloader.refl.hh>

VK_CLASS()
class VKE_API vkDynamicColliderStateXMLLoader : public VK_SUPER(vkBaseColliderStateXMLLoader)
{
  VK_CLASS_GEN;
public:
  vkDynamicColliderStateXMLLoader();
  virtual ~vkDynamicColliderStateXMLLoader();

  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *GetLoadingClass() const;

};

