#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkresourcemanager.hh>
#include <valkyrie/loaders/vkstaticmeshassetloader.refl.hh>

class vkSubMesh;
struct iIndexBuffer;

VK_CLASS()
class VKE_API vkStaticMeshAssetLoader : public VK_SUPER(iAssetLoader)
{
  VK_CLASS_GEN_OBJECT;
public:
  vkStaticMeshAssetLoader();
  virtual ~vkStaticMeshAssetLoader();

  virtual bool CanLoad(const vkString &typeID, const vkResourceLocator &locator, iObject *userData = 0) const;
  const vkClass *EvalClass(vkAssetInputStream &inputStream, const vkResourceLocator &locator, iObject *userData = 0) const;

  iObject *Load(vkAssetInputStream &inputStream, const vkResourceLocator &locator, iObject *userData = 0) const;

private:
  vkSubMesh *ReadSubMesh(vkAssetInputStream &inputStream, std::vector<iIndexBuffer*> &globalIndexBuffers, const vkResourceLocator &locator, iObject *userData = 0) const;
};