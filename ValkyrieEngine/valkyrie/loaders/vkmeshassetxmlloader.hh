#pragma once


#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkresourcemanager.hh>
#include <valkyrie/loaders/vkmeshassetxmlloader.refl.hh>


VK_CLASS()
class VKE_API vkMeshAssetXMLLoader : public VK_SUPER(vkBaseXMLLoader)
{
  VK_CLASS_GEN_OBJECT;
public:
  vkMeshAssetXMLLoader();
  virtual ~vkMeshAssetXMLLoader();


  bool CanLoad(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData = 0) const;
  const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData = 0) const;
  iObject *Load(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData = 0) const;

};
