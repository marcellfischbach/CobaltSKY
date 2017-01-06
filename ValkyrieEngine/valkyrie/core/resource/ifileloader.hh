#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/core/resource/ifileloader.refl.hh>

struct IFile;
class vkResourceLocator;

VK_INTERFACE()
struct VKE_API IFileLoader : public IObject
{
  VK_CLASS_GEN;

  virtual bool CanLoad(IFile *file, const vkResourceLocator &locator, IObject *userData = 0) const = 0;
  virtual const vkClass *EvalClass(IFile *file, const vkResourceLocator &locator, IObject *userData = 0) const = 0;
  virtual IObject *Load(IFile *file, const vkResourceLocator &locator, IObject *userData = 0) const = 0;
};
