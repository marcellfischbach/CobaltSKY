#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/cspngimageassetloader.refl.hh>





CS_CLASS()
class CSE_API csPNGImageAssetLoader : public CS_SUPER(iAssetLoader)
{
  CS_CLASS_GEN_OBJECT;
public:
  csPNGImageAssetLoader();
  virtual ~csPNGImageAssetLoader();

  bool CanLoad(const csString &typeID, const csResourceLocator &locator, iObject *userData = 0)const ;
  virtual const csClass *EvalClass(csAssetInputStream &inputStream, const csResourceLocator &locator, iObject *userData = 0) const;
  iObject *Load(csAssetInputStream &inputStream, const csResourceLocator &locator, iObject *userData = 0) const;
};



