#pragma once
#include <cobalt/csexport.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/cssubmeshassetloader.refl.hh>

CS_CLASS()
class CSE_API csSubMeshAssetLoader : public CS_SUPER(iAssetLoader)
{
  CS_CLASS_GEN_OBJECT;
public:
  csSubMeshAssetLoader();
  virtual ~csSubMeshAssetLoader();

  bool CanLoad(const std::string &typeID, const csResourceLocator &locator, iObject *userData = 0)const;
  virtual const csClass *EvalClass(csAssetInputStream &inputStream, const csResourceLocator &locator, iObject *userData = 0) const;
  iObject *Load(csAssetInputStream &inputStream, const csResourceLocator &locator, iObject *userData = 0) const;
};

