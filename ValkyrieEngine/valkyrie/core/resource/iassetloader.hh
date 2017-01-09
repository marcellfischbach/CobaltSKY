#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/core/resource/iassetloader.refl.hh>

class vkAssetInputStream;
class vkResourceLocator;

VK_INTERFACE()
struct VKE_API iAssetLoader : public iObject
{
  VK_CLASS_GEN;
  virtual ~iAssetLoader() { }

  virtual bool CanLoad(const vkString &typeID, const vkResourceLocator &locator, iObject *userData = 0) const = 0;
  virtual const vkClass *EvalClass(vkAssetInputStream &inputStream, const vkResourceLocator &locator, iObject *userData = 0) const = 0;
  virtual iObject *Load(vkAssetInputStream &inputStream, const vkResourceLocator &locator, iObject *userData = 0) const = 0;
};
