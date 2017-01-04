#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/core/resource/ifileloader.refl.hh>

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
