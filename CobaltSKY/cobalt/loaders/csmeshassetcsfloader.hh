#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/csmeshassetcsfloader.refl.hh>


CS_CLASS()
class CSE_API csMeshAssetCSFLoader : public CS_SUPER(csBaseCSFLoader)
{
  CS_CLASS_GEN_OBJECT;
public:
  csMeshAssetCSFLoader();
  virtual ~csMeshAssetCSFLoader();


  bool CanLoad(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;
  const csClass *EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;
  iObject *Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;

};
