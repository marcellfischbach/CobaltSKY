#pragma once

#include <valkyrie/csexport.hh>
#include <valkyrie/core/csclass.hh>
#include <valkyrie/core/resource/iassetloader.refl.hh>

class csAssetInputStream;
class csResourceLocator;

CS_INTERFACE()
struct CSE_API iAssetLoader : public iObject
{
  CS_CLASS_GEN;
  virtual ~iAssetLoader() { }

  virtual bool CanLoad(const csString &typeID, const csResourceLocator &locator, iObject *userData = 0) const = 0;
  virtual const csClass *EvalClass(csAssetInputStream &inputStream, const csResourceLocator &locator, iObject *userData = 0) const = 0;
  virtual iObject *Load(csAssetInputStream &inputStream, const csResourceLocator &locator, iObject *userData = 0) const = 0;
};
