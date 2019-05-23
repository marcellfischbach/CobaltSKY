#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/core/resource/iassetloader.refl.hh>


namespace cs
{
class AssetInputStream;
class ResourceLocator;
class ResourceWrapper;

CS_CLASS()
struct CSE_API iAssetLoader : public cs::iObject
{
  CS_CLASS_GEN;
  virtual ~iAssetLoader() { }

  virtual bool CanLoad(const std::string& typeID, const cs::ResourceLocator& locator, cs::iObject* userData = nullptr) const = 0;
  virtual const cs::Class* EvalClass(cs::AssetInputStream& inputStream, const cs::ResourceLocator& locator, cs::iObject* userData = nullptr) const = 0;
  virtual cs::ResourceWrapper* Load(cs::AssetInputStream& inputStream, const cs::ResourceLocator& locator, cs::iObject* userData = nullptr) const = 0;
};

}