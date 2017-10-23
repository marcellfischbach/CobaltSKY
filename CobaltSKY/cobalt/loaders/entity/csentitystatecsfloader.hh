#pragma once
#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/entity/csentitystatecsfloader.refl.hh>

CS_CLASS()
/**
* \ingroup loading
*/
class CSE_API csEntityStateCSFLoader : public CS_SUPER(csBaseCSFLoader)
{
  CS_CLASS_GEN;
public:
  csEntityStateCSFLoader();
  virtual ~csEntityStateCSFLoader();


  virtual bool CanLoad(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual const csClass *EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual iObject *Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;

  virtual const csClass *GetLoadingClass() const;

};
