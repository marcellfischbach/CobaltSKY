#pragma once
#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/entity/csentitystatexmlloader.hh>
#include <cobalt/loaders/entity/csspatialstatexmlloader.refl.hh>

CS_CLASS()
/**
* \ingroup loading
*/
class CSE_API csSpatialStateXMLLoader : public CS_SUPER(csEntityStateXMLLoader)
{
  CS_CLASS_GEN;
public:
  csSpatialStateXMLLoader();
  virtual ~csSpatialStateXMLLoader();

  virtual iObject *Load(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual const csClass *EvalClass(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual const csClass *GetLoadingClass() const;

};
