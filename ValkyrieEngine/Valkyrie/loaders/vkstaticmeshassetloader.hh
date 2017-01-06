#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkresourcemanager.hh>
#include <Valkyrie/loaders/vkstaticmeshassetloader.refl.hh>

class vkSubMesh;
struct IIndexBuffer;

VK_CLASS()
class VKE_API vkStaticMeshAssetLoader : public VK_SUPER(IAssetLoader)
{
  VK_CLASS_GEN_OBJECT;
public:
  vkStaticMeshAssetLoader();
  virtual ~vkStaticMeshAssetLoader();

  virtual bool CanLoad(const vkString &typeID, const vkResourceLocator &locator, IObject *userData = 0) const;
  const vkClass *EvalClass(vkAssetInputStream &inputStream, const vkResourceLocator &locator, IObject *userData = 0) const;

  IObject *Load(vkAssetInputStream &inputStream, const vkResourceLocator &locator, IObject *userData = 0) const;

private:
  vkSubMesh *ReadSubMesh(vkAssetInputStream &inputStream, std::vector<IIndexBuffer*> &globalIndexBuffers, const vkResourceLocator &locator, IObject *userData = 0) const;
};