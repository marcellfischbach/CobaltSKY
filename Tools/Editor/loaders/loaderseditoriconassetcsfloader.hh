#pragma once

#include <cobalt/core/csclass.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <loaders/loaderseditoriconassetcsfloader.refl.hh>

CS_CLASS()
class LoadersEditorIconAssetCSFLoader : public CS_SUPER(csBaseCSFLoader)
{
  CS_CLASS_GEN;
public:
  LoadersEditorIconAssetCSFLoader();
  virtual ~LoadersEditorIconAssetCSFLoader();

  virtual bool CanLoad(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual const csClass *EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual iObject *Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;


};
