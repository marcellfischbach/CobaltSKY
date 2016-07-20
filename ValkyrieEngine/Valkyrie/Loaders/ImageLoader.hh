#pragma once


#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/ResourceManager.hh>
#include <Valkyrie/Loaders/ImageLoader.refl.hh>


/**
* \ingroup loading
*/
VK_CLASS();
class VKE_API vkPNGImageFileLoader : public IFileLoader
{
  VK_CLASS_GEN_OBJECT;

public:
  vkPNGImageFileLoader();
  virtual ~vkPNGImageFileLoader();

  virtual bool CanLoad(IFile *file, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual IObject *Load(IFile *file, const vkResourceLocator &locator, IObject *userData = 0) const;

};




VK_CLASS();
class VKE_API vkPNGImageAssetLoader : public IAssetLoader
{
  VK_CLASS_GEN_OBJECT;
public:
  vkPNGImageAssetLoader();
  virtual ~vkPNGImageAssetLoader();

  bool CanLoad(const vkString &typeID, const vkResourceLocator &locator, IObject *userData = 0);
  IObject *Load(vkAssetInputStream &inputStream, const vkResourceLocator &locator, IObject *userData = 0);
};



