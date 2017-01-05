#pragma once


#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkresourcemanager.hh>
#include <Valkyrie/Loaders/vkmeshassetxmlloader.refl.hh>


VK_CLASS()
class VKE_API vkMeshAssetXMLLoader : public VK_SUPER(vkBaseXMLLoader)
{
  VK_CLASS_GEN_OBJECT;
public:
  vkMeshAssetXMLLoader();
  virtual ~vkMeshAssetXMLLoader();


  bool CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;

};
