#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkresourcemanager.hh>
#include <valkyrie/loaders/vkpngimagefileloader.refl.hh>

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

