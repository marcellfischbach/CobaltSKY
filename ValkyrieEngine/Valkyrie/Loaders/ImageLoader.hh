#pragma once


#include <Valkyrie/vkexport.hh>
#include <Valkyrie/Core/vkresourcemanager.hh>
#include <Valkyrie/Loaders/ImageLoader.refl.hh>


/**
* \ingroup loading
*/
VK_CLASS()
class VKE_API vkPNGImageFileLoader : public VK_SUPER(IFileLoader)
{
  VK_CLASS_GEN_OBJECT;

public:
  vkPNGImageFileLoader();
  virtual ~vkPNGImageFileLoader();

  virtual bool CanLoad(IFile *file, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *EvalClass(IFile *file, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual IObject *Load(IFile *file, const vkResourceLocator &locator, IObject *userData = 0) const;

};




VK_CLASS()
class VKE_API vkPNGImageAssetLoader : public VK_SUPER(IAssetLoader)
{
  VK_CLASS_GEN_OBJECT;
public:
  vkPNGImageAssetLoader();
  virtual ~vkPNGImageAssetLoader();

  bool CanLoad(const vkString &typeID, const vkResourceLocator &locator, IObject *userData = 0)const ;
  virtual const vkClass *EvalClass(vkAssetInputStream &inputStream, const vkResourceLocator &locator, IObject *userData = 0) const;
  IObject *Load(vkAssetInputStream &inputStream, const vkResourceLocator &locator, IObject *userData = 0) const;
};



