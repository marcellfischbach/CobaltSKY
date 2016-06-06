#pragma once


#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/ResourceManager.hh>
#include <Valkyrie/Loaders/MaterialInstanceLoader.refl.hh>

class vkMaterialInstance;

VK_CLASS()
class VKE_API vkMaterialInstanceAssetLoader : public IAssetLoader
{
  VK_CLASS_GEN_OBJECT;
public:
  vkMaterialInstanceAssetLoader();
  virtual ~vkMaterialInstanceAssetLoader();

  virtual bool CanLoad(const vkString &typeID, const vkString &name, const vkResourceLocator &locator, IObject *userData = 0);
  virtual IObject *Load(vkAssetInputStream &inputStream, const vkResourceLocator &locator, IObject *userData = 0);

};
