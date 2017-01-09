#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/resource/ifileloader.hh>
#include <valkyrie/core/resource/vkassetfileloader.refl.hh>

VK_CLASS()
class VKE_API vkAssetFileLoader : public VK_SUPER(iFileLoader)
{
  VK_CLASS_GEN_OBJECT;
public:
  vkAssetFileLoader();
  virtual ~vkAssetFileLoader();

  virtual bool CanLoad(iFile *file, const vkResourceLocator &locator, iObject *userData = 0) const;
  virtual const vkClass *EvalClass(iFile *file, const vkResourceLocator &locator, iObject *userData = 0) const;
  virtual iObject *Load(iFile *file, const vkResourceLocator &locator, iObject *userData = 0) const;

};
