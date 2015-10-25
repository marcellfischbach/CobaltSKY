#pragma once


#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/ResourceManager.hh>
#include <Valkyrie/Loaders/ImageLoader.refl.hh>


VK_CLASS();
class VKE_API vkPNGImageLoader : public IFileLoader
{
  VK_CLASS_GEN_OBJECT;

public:
  vkPNGImageLoader();
  virtual ~vkPNGImageLoader();

  virtual bool CanLoad(IFile *file, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual IObject *Load(IFile *file, const vkResourceLocator &locator, IObject *userData = 0) const;

};