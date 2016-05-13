#pragma once


#include <Valkyrie/Core/ResourceManager.hh>
#include <AssetManager/EditorIconLoader.refl.hh>


VK_CLASS()
class EditorIconLoader : public IAssetLoader
{
  VK_CLASS_GEN_OBJECT;
public:
  EditorIconLoader() : IAssetLoader() { }
  virtual ~EditorIconLoader() { }

  virtual bool CanLoad(const vkString &typeID, const vkString &name, const vkResourceLocator &locator, IObject *userData = 0);

  virtual IObject *Load(vkAssetInputStream &inputStream, const vkResourceLocator &locator, IObject *userData = 0);

};
