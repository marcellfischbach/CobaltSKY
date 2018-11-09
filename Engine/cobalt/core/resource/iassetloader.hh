#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/core/resource/iassetloader.refl.hh>

class csAssetInputStream;
class csResourceLocator;
class csResourceWrapper;

CS_INTERFACE()
struct CSE_API iAssetLoader : public iObject
{
  CS_CLASS_GEN;
  virtual ~iAssetLoader() { }

  virtual bool CanLoad(const std::string &typeID, const csResourceLocator &locator, iObject *userData = nullptr) const = 0;
  virtual const csClass *EvalClass(csAssetInputStream &inputStream, const csResourceLocator &locator, iObject *userData = nullptr) const = 0;
  virtual csResourceWrapper *Load(csAssetInputStream &inputStream, const csResourceLocator &locator, iObject *userData = nullptr) const = 0;
};
