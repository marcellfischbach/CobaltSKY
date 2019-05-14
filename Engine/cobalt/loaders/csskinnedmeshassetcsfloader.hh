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


  bool CanLoad(const csfEntry *element, const csResourceLocator &locator, cs::iObject *userData = nullptr) const;
  const cs::Class *EvalClass(const csfEntry *element, const csResourceLocator &locator, cs::iObject *userData = nullptr) const;
  csResourceWrapper *Load(const csfEntry *element, const csResourceLocator &locator, cs::iObject *userData = nullptr) const;

};
