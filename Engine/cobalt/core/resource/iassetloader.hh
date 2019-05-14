#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/core/resource/iassetloader.refl.hh>

class csAssetInputStream;
class csResourceLocator;
class csResourceWrapper;

CS_CLASS()
struct CSE_API iAssetLoader : public cs::iObject
{
  CS_CLASS_GEN;
  virtual ~iAssetLoader() { }

  virtual bool CanLoad(const std::string &typeID, const csResourceLocator &locator, cs::iObject *userData = nullptr) const = 0;
  virtual const cs::Class *EvalClass(csAssetInputStream &inputStream, const csResourceLocator &locator, cs::iObject *userData = nullptr) const = 0;
  virtual csResourceWrapper *Load(csAssetInputStream &inputStream, const csResourceLocator &locator, cs::iObject *userData = nullptr) const = 0;
};
