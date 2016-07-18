#pragma once


#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/ResourceManager.hh>
#include <Valkyrie/Loaders/ImageLoader.refl.hh>


/**
* \ingroup loading
*/
VK_CLASS();
class VKE_API vkPNGImageLoader : public IFileLoader
{
  VK_CLASS_GEN_OBJECT;

public:
  vkPNGImageLoader();
  virtual ~vkPNGImageLoader();

  virtual bool CanLoad(IFile *file, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual IObject *Load(IFile *file, const vkResourceLocator &locator, IObject *userData = 0) const;

};




VK_CLASS();
class VKE_API vkImageDataLoader : public IAssetLoader
{
  VK_CLASS_GEN_OBJECT;
public:
  vkImageDataLoader();
  virtual ~vkImageDataLoader();

  bool CanLoad(const vkString &typeID, const vkString &name, const vkResourceLocator &locator, IObject *userData = 0);
  IObject *Load(vkAssetInputStream &inputStream, const vkResourceLocator &locator, IObject *userData = 0);
};


VK_INTERFACE()
struct VKE_API IImageDataLoader : public IObject
{
  VK_CLASS_GEN;
  virtual ~IImageDataLoader() { }

  virtual bool CanLoad(const vkString &format, const vkResourceLocator &locator, IObject *userData = 0) = 0;
  virtual IObject *Load(vkAssetInputStream &inputStream, const vkResourceLocator &locator, IObject *userData = 0) = 0;
};


VK_CLASS()
class VKE_API vkPNGImageDataLoader : public IImageDataLoader
{
  VK_CLASS_GEN_OBJECT;
public:
  vkPNGImageDataLoader();
  virtual ~vkPNGImageDataLoader();

  bool CanLoad(const vkString &format, const vkResourceLocator &locator, IObject *userData = 0);
  IObject *Load(vkAssetInputStream &inputStream, const vkResourceLocator &locator, IObject *userData = 0);
};