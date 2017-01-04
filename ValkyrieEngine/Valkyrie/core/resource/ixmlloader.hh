#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/core/resource/ixmlloader.refl.hh>

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

