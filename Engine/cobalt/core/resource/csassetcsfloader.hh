#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/resource/csbasecsfloader.hh>
#include <cobalt/core/resource/csassetcsfloader.refl.hh>

CS_CLASS()
class CSE_API csAssetCSFLoader : public CS_SUPER(csBaseCSFLoader)
{
  CS_CLASS_GEN;
public:
  csAssetCSFLoader();
  virtual ~csAssetCSFLoader();

  virtual bool CanLoad(const csfEntry *entry, const csResourceLocator &locator, cs::iObject *userData = nullptr) const;
  virtual const cs::Class *EvalClass(const csfEntry *entry, const csResourceLocator &locator, cs::iObject *userData = nullptr) const;
  virtual csResourceWrapper *Load(const csfEntry *entry, const csResourceLocator &locator, cs::iObject *userData = nullptr) const;

};
