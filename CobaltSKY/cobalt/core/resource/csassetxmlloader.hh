#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/resource/csbasexmlloader.hh>
#include <cobalt/core/resource/csassetxmlloader.refl.hh>

CS_CLASS()
class CSE_API csAssetXMLLoader : public CS_SUPER(csBaseXMLLoader)
{
  CS_CLASS_GEN;
public:
  csAssetXMLLoader();
  virtual ~csAssetXMLLoader();

  virtual bool CanLoad(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual const csClass *EvalClass(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual iObject *Load(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;

};
