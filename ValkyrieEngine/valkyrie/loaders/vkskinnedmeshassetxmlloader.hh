#pragma once
#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkresourcemanager.hh>
#include <valkyrie/loaders/vkskinnedmeshassetxmlloader.refl.hh>

VK_CLASS()
class VKE_API vkSkinnedMeshAssetXMLLoader : public VK_SUPER(vkBaseXMLLoader)
{
  VK_CLASS_GEN_OBJECT;
public:
  vkSkinnedMeshAssetXMLLoader();
  virtual ~vkSkinnedMeshAssetXMLLoader();


  bool CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;

};
