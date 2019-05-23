#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/csmeshassetcsfloader.refl.hh>

namespace cs
{

CS_CLASS()
class CSE_API MeshAssetCSFLoader : public CS_SUPER(cs::BaseCSFLoader)
{
  CS_CLASS_GEN_OBJECT;
public:
  MeshAssetCSFLoader();
  virtual ~MeshAssetCSFLoader();


  bool CanLoad(const cs::file::Entry * entry, const cs::ResourceLocator & locator, cs::iObject * userData = nullptr) const;
  const cs::Class * EvalClass(const cs::file::Entry * entry, const cs::ResourceLocator & locator, cs::iObject * userData = nullptr) const;
  cs::ResourceWrapper * Load(const cs::file::Entry * entry, const cs::ResourceLocator & locator, cs::iObject * userData = nullptr) const;

};

}
