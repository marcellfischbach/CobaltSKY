#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/entity/blueprint/csblueprintcsfloader.refl.hh>



CS_CLASS()
class CSE_API csBlueprintCSFLoader : public CS_SUPER(csBaseCSFLoader)
{
  CS_CLASS_GEN_OBJECT;
public:
  csBlueprintCSFLoader();
  virtual ~csBlueprintCSFLoader();

  virtual bool CanLoad(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual const csClass *EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual iObject *Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;

};
