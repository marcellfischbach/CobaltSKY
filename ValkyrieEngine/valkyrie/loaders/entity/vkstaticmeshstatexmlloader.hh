#pragma once
#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/core/vkresourcemanager.hh>
#include <valkyrie/loaders/entity/vkrenderstatexmlloader.hh>
#include <valkyrie/loaders/entity/vkstaticmeshstatexmlloader.refl.hh>


VK_CLASS()
/**
* \ingroup loading
*/
class VKE_API vkStaticMeshStateXMLLoader : public VK_SUPER(vkRenderStateXMLLoader)
{
  VK_CLASS_GEN;
public:
  vkStaticMeshStateXMLLoader();
  virtual ~vkStaticMeshStateXMLLoader();

  virtual iObject *Load(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData = 0) const;
  virtual const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData = 0) const;
  virtual const vkClass *GetLoadingClass() const;

};

