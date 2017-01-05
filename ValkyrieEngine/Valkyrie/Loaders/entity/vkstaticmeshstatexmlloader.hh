#pragma once
#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/core/vkresourcemanager.hh>
#include <Valkyrie/Loaders/entity/vkrenderstatexmlloader.hh>
#include <Valkyrie/Loaders/entity/vkstaticmeshstatexmlloader.refl.hh>


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

  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *GetLoadingClass() const;

};

