#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/core/resource/ixmlloader.refl.hh>

class TiXmlElement;
class vkResourceLocator;


VK_INTERFACE()
struct VKE_API iXMLLoader : public iObject
{
  VK_CLASS_GEN;

  virtual bool CanLoad(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData = 0) const = 0;
  virtual const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData = 0) const = 0;
  virtual iObject *Load(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData = 0) const = 0;
};

