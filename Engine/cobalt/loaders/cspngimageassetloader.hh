#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/cspngimageassetloader.refl.hh>


namespace cs
{


CS_CLASS()
class CSE_API PNGImageAssetLoader : public CS_SUPER(cs::iAssetLoader)
{
  CS_CLASS_GEN_OBJECT;
public:
  PNGImageAssetLoader();
  virtual ~PNGImageAssetLoader();

  bool CanLoad(const std::string & typeID, const cs::ResourceLocator & locator, cs::iObject * userData = nullptr)const;
  virtual const cs::Class * EvalClass(cs::AssetInputStream & inputStream, const cs::ResourceLocator & locator, cs::iObject * userData = nullptr) const;
  cs::ResourceWrapper * Load(cs::AssetInputStream & inputStream, const cs::ResourceLocator & locator, cs::iObject * userData = nullptr) const;
};




}

