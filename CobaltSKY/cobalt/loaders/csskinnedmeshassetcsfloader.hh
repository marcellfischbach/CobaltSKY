#pragma once
#include <cobalt/csexport.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/csskinnedmeshassetcsfloader.refl.hh>

CS_CLASS()
class CSE_API csSkinnedMeshAssetCSFLoader : public CS_SUPER(csBaseCSFLoader)
{
  CS_CLASS_GEN_OBJECT;
public:
  csSkinnedMeshAssetCSFLoader();
  virtual ~csSkinnedMeshAssetCSFLoader();


  bool CanLoad(const csfEntry *element, const csResourceLocator &locator, iObject *userData = 0) const;
  const csClass *EvalClass(const csfEntry *element, const csResourceLocator &locator, iObject *userData = 0) const;
  iObject *Load(const csfEntry *element, const csResourceLocator &locator, iObject *userData = 0) const;

};
