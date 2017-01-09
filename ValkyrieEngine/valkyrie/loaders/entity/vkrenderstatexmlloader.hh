#pragma once
#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/core/vkresourcemanager.hh>
#include <valkyrie/loaders/entity/vkspatialstatexmlloader.hh>
#include <valkyrie/loaders/entity/vkrenderstatexmlloader.refl.hh>


VK_CLASS()
/**
* \ingroup loading
*/
class VKE_API vkRenderStateXMLLoader : public VK_SUPER(vkSpatialStateXMLLoader)
{
  VK_CLASS_GEN;
public:
  vkRenderStateXMLLoader();
  virtual ~vkRenderStateXMLLoader();

  virtual iObject *Load(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData = 0) const;
  virtual const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData = 0) const;
  virtual const vkClass *GetLoadingClass() const;

};
