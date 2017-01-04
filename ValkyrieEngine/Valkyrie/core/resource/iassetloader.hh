#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/core/resource/iassetloader.refl.hh>

class vkAssetInputStream;
class vkResourceLocator;

VK_INTERFACE()
struct VKE_API IAssetLoader : public IObject
{
  VK_CLASS_GEN;
  virtual ~IAssetLoader() { }

  virtual bool CanLoad(const vkString &typeID, const vkResourceLocator &locator, IObject *userData = 0) const = 0;
  virtual const vkClass *EvalClass(vkAssetInputStream &inputStream, const vkResourceLocator &locator, IObject *userData = 0) const = 0;
  virtual IObject *Load(vkAssetInputStream &inputStream, const vkResourceLocator &locator, IObject *userData = 0) const = 0;
};
