#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/resource/ifileloader.hh>
#include <valkyrie/core/resource/vkxmlfileloader.refl.hh>

VK_CLASS()
class VKE_API vkXMLFileLoader : public VK_SUPER(iFileLoader)
{
  VK_CLASS_GEN_OBJECT;
public:
  vkXMLFileLoader();
  virtual ~vkXMLFileLoader();
  virtual bool CanLoad(iFile *file, const vkResourceLocator &locator, iObject *userData = 0) const;
  virtual const vkClass *EvalClass(iFile *file, const vkResourceLocator &locator, iObject *userData = 0) const;
  iObject *Load(iFile *file, const vkResourceLocator &locator, iObject *userData = 0) const;

};

