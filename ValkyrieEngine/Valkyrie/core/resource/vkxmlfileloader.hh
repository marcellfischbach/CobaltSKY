#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/resource/ifileloader.hh>
#include <Valkyrie/core/resource/vkxmlfileloader.refl.hh>

VK_CLASS()
class VKE_API vkXMLFileLoader : public VK_SUPER(IFileLoader)
{
  VK_CLASS_GEN_OBJECT;
public:
  vkXMLFileLoader();
  virtual ~vkXMLFileLoader();
  virtual bool CanLoad(IFile *file, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *EvalClass(IFile *file, const vkResourceLocator &locator, IObject *userData = 0) const;
  IObject *Load(IFile *file, const vkResourceLocator &locator, IObject *userData = 0) const;

};

