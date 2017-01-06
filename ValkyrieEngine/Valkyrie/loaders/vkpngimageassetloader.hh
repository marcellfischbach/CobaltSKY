#pragma once


#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkresourcemanager.hh>
#include <Valkyrie/loaders/vkpngimageassetloader.refl.hh>





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



