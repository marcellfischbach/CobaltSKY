#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <csfile/csffile.hh>

#include <cobalt/core/resource/icsfloader.refl.hh>


namespace cs
{
class ResourceLocator;
class ResourceWrapper;

CS_CLASS()
struct CSE_API iCSFLoader : public cs::iObject
{
  CS_CLASS_GEN;

  virtual bool CanLoad(const csfEntry* entry, const cs::ResourceLocator& locator, cs::iObject* userData = nullptr) const = 0;
  virtual const cs::Class* EvalClass(const csfEntry* entry, const cs::ResourceLocator& locator, cs::iObject* userData = nullptr) const = 0;
  virtual cs::ResourceWrapper* Load(const csfEntry* entry, const cs::ResourceLocator& locator, cs::iObject* userData = nullptr) const = 0;
};

}