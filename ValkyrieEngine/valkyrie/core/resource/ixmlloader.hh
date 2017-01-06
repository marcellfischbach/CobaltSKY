#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/core/resource/ixmlloader.refl.hh>

class TiXmlElement;
class vkResourceLocator;


VK_INTERFACE()
struct VKE_API IXMLLoader : public IObject
{
  VK_CLASS_GEN;

  virtual bool CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const = 0;
  virtual const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const = 0;
  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const = 0;
};

