#pragma once
#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/entity/csentitystatecsfloader.hh>
#include <cobalt/loaders/entity/csspatialstatecsfloader.refl.hh>

CS_CLASS()
/**
* \ingroup loading
*/
class CSE_API csSpatialStateCSFLoader : public CS_SUPER(csEntityStateCSFLoader)
{
  CS_CLASS_GEN;
public:
  csSpatialStateCSFLoader();
  virtual ~csSpatialStateCSFLoader();

  virtual iObject *Load(const csfEntry *element, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual const csClass *EvalClass(const csfEntry *element, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual const csClass *GetLoadingClass() const;

};
