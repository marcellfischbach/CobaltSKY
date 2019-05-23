#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/core/resource/csbasecsfloader.hh>
#include <cobalt/loaders/cssamplerassetcsfloader.refl.hh>


namespace cs
{
/**
* \ingroup loading
*/

CS_CLASS()
class CSE_API SamplerAssetCSFLoader : public CS_SUPER(cs::BaseCSFLoader)
{
  CS_CLASS_GEN;
public:
  SamplerAssetCSFLoader();
  virtual ~SamplerAssetCSFLoader();

  virtual bool CanLoad(const csfEntry * entry, const cs::ResourceLocator & locator, cs::iObject * userData = nullptr) const;
  virtual const cs::Class * EvalClass(const csfEntry * entry, const cs::ResourceLocator & locator, cs::iObject * userData = nullptr) const;
  virtual cs::ResourceWrapper * Load(const csfEntry * entry, const cs::ResourceLocator & locator, cs::iObject * userData = nullptr) const;

};

}