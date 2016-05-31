#pragma once


#include <Valkyrie/Core/ResourceManager.hh>
#include <ShaderGraph/MetaDataLoader.refl.hh>


VK_CLASS()
class MetaDataLoader : public IAssetLoader
{
  VK_CLASS_GEN_OBJECT;
public:
  MetaDataLoader();
  virtual ~MetaDataLoader();

  virtual bool CanLoad(const vkString &typeID, const vkString &name, const vkResourceLocator &locator, IObject *userData = 0);

  virtual IObject *Load(vkAssetInputStream &inputStream, const vkResourceLocator &locator, IObject *userData = 0);

};
