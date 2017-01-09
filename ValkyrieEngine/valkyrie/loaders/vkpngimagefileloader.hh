#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkresourcemanager.hh>
#include <valkyrie/loaders/vkpngimagefileloader.refl.hh>

/**
* \ingroup loading
*/
VK_CLASS()
class VKE_API vkPNGImageFileLoader : public VK_SUPER(iFileLoader)
{
  VK_CLASS_GEN_OBJECT;

public:
  vkPNGImageFileLoader();
  virtual ~vkPNGImageFileLoader();

  virtual bool CanLoad(iFile *file, const vkResourceLocator &locator, iObject *userData = 0) const;
  virtual const vkClass *EvalClass(iFile *file, const vkResourceLocator &locator, iObject *userData = 0) const;
  virtual iObject *Load(iFile *file, const vkResourceLocator &locator, iObject *userData = 0) const;

};

