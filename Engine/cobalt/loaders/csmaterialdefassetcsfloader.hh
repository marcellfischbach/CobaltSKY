#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/csmaterialdefassetcsfloader.refl.hh>

/**
* \ingroup loading
*/
CS_CLASS()
class CSE_API csMaterialDefAssetCSFLoader : public CS_SUPER(csBaseCSFLoader)
{
  CS_CLASS_GEN;
public:
  csMaterialDefAssetCSFLoader();
  virtual ~csMaterialDefAssetCSFLoader();

  virtual bool CanLoad(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = nullptr) const;
  virtual const csClass *EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = nullptr) const;
  virtual csResourceWrapper *Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = nullptr) const;


};

