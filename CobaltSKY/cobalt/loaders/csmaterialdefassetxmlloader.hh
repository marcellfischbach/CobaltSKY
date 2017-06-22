#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/csmaterialdefassetxmlloader.refl.hh>

/**
* \ingroup loading
*/
CS_CLASS()
class CSE_API csMaterialDefAssetXMLLoader : public CS_SUPER(csBaseXMLLoader)
{
  CS_CLASS_GEN;
public:
  csMaterialDefAssetXMLLoader();
  virtual ~csMaterialDefAssetXMLLoader();

  virtual bool CanLoad(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual const csClass *EvalClass(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual iObject *Load(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;


};

