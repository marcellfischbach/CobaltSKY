#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/entity/csentitycsfloader.refl.hh>

CS_CLASS()
/**
* \ingroup loading
*/
class CSE_API csEntityCSFLoader : public CS_SUPER(csBaseCSFLoader)
{
  CS_CLASS_GEN;
public:
  csEntityCSFLoader();
  virtual ~csEntityCSFLoader();


  virtual bool CanLoad(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual const csClass *EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual iObject *Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;

  virtual const csClass *GetLoadingClass() const;
};