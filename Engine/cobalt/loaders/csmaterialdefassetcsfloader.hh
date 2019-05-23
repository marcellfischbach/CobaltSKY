#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/csmaterialdefassetcsfloader.refl.hh>

namespace cs
{
/**
* \ingroup loading
*/
CS_CLASS()
class CSE_API MaterialDefAssetCSFLoader : public CS_SUPER(cs::BaseCSFLoader)
{
  CS_CLASS_GEN;
public:
  MaterialDefAssetCSFLoader();
  virtual ~MaterialDefAssetCSFLoader();

  virtual bool CanLoad(const csfEntry * entry, const cs::ResourceLocator & locator, cs::iObject * userData = nullptr) const;
  virtual const cs::Class * EvalClass(const csfEntry * entry, const cs::ResourceLocator & locator, cs::iObject * userData = nullptr) const;
  virtual cs::ResourceWrapper * Load(const csfEntry * entry, const cs::ResourceLocator & locator, cs::iObject * userData = nullptr) const;


};


}