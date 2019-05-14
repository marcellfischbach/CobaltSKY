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

  bool CanLoad(const std::string &typeID, const csResourceLocator &locator, cs::iObject *userData = nullptr)const ;
  virtual const cs::Class *EvalClass(csAssetInputStream &inputStream, const csResourceLocator &locator, cs::iObject *userData = nullptr) const;
  csResourceWrapper *Load(csAssetInputStream &inputStream, const csResourceLocator &locator, cs::iObject *userData = nullptr) const;
};



