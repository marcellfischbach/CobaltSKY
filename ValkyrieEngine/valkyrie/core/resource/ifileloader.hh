#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/core/resource/ifileloader.refl.hh>

struct iFile;
class vkResourceLocator;

VK_INTERFACE()
struct VKE_API iFileLoader : public iObject
{
  VK_CLASS_GEN;

  virtual bool CanLoad(iFile *file, const vkResourceLocator &locator, iObject *userData = 0) const = 0;
  virtual const vkClass *EvalClass(iFile *file, const vkResourceLocator &locator, iObject *userData = 0) const = 0;
  virtual iObject *Load(iFile *file, const vkResourceLocator &locator, iObject *userData = 0) const = 0;
};
