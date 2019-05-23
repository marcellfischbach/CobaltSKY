#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/core/resource/ifileloader.refl.hh>


namespace cs
{
struct iFile;

class ResourceLocator;
class ResourceWrapper;

CS_CLASS()
struct CSE_API iFileLoader : public cs::iObject
{
  CS_CLASS_GEN;

  virtual bool CanLoad(cs::iFile* file, const cs::ResourceLocator& locator) const = 0;
  virtual const cs::Class* EvalClass(cs::iFile* file, const cs::ResourceLocator& locator) const = 0;
  virtual cs::ResourceWrapper* Load(cs::iFile* file, const cs::ResourceLocator& locator) const = 0;
};

}