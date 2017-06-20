#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/entity/csentityxmlloader.refl.hh>

CS_CLASS()
/**
* \ingroup loading
*/
class CSE_API csEntityXMLLoader : public CS_SUPER(csBaseXMLLoader)
{
  CS_CLASS_GEN;
public:
  csEntityXMLLoader();
  virtual ~csEntityXMLLoader();


  virtual bool CanLoad(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual const csClass *EvalClass(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual iObject *Load(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;

  virtual const csClass *GetLoadingClass() const;
};
