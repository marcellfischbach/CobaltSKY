#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/resource/ifileloader.hh>
#include <valkyrie/core/resource/vkassetfileloader.refl.hh>

VK_CLASS()
class VKE_API vkAssetFileLoader : public VK_SUPER(IFileLoader)
{
  VK_CLASS_GEN_OBJECT;
public:
  vkAssetFileLoader();
  virtual ~vkAssetFileLoader();

  virtual bool CanLoad(IFile *file, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *EvalClass(IFile *file, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual IObject *Load(IFile *file, const vkResourceLocator &locator, IObject *userData = 0) const;

};
