#pragma once
#include <cobalt/csexport.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/csskinnedmeshassetcsfloader.refl.hh>


namespace cs
{

CS_CLASS()
class CSE_API SkinnedMeshAssetCSFLoader : public CS_SUPER(cs::BaseCSFLoader)
{
  CS_CLASS_GEN_OBJECT;
public:
  SkinnedMeshAssetCSFLoader();
  virtual ~SkinnedMeshAssetCSFLoader();


  bool CanLoad(const csfEntry * element, const cs::ResourceLocator & locator, cs::iObject * userData = nullptr) const;
  const cs::Class * EvalClass(const csfEntry * element, const cs::ResourceLocator & locator, cs::iObject * userData = nullptr) const;
  cs::ResourceWrapper * Load(const csfEntry * element, const cs::ResourceLocator & locator, cs::iObject * userData = nullptr) const;

};

}
