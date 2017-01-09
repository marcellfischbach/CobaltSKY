#pragma once
#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkresourcemanager.hh>
#include <valkyrie/loaders/vksubmeshassetxmlloader.refl.hh>

VK_CLASS()
class VKE_API vkSubMeshAssetLoader : public VK_SUPER(iAssetLoader)
{
  VK_CLASS_GEN_OBJECT;
public:
  vkSubMeshAssetLoader();
  virtual ~vkSubMeshAssetLoader();

  bool CanLoad(const vkString &typeID, const vkResourceLocator &locator, iObject *userData = 0)const;
  virtual const vkClass *EvalClass(vkAssetInputStream &inputStream, const vkResourceLocator &locator, iObject *userData = 0) const;
  iObject *Load(vkAssetInputStream &inputStream, const vkResourceLocator &locator, iObject *userData = 0) const;
};

