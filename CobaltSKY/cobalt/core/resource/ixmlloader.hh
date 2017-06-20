#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/core/resource/ixmlloader.refl.hh>

class TiXmlElement;
class csResourceLocator;


CS_INTERFACE()
struct CSE_API iXMLLoader : public iObject
{
  CS_CLASS_GEN;

  virtual bool CanLoad(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const = 0;
  virtual const csClass *EvalClass(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const = 0;
  virtual iObject *Load(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const = 0;
};

