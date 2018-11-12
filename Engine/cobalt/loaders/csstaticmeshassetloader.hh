#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/csstaticmeshassetloader.refl.hh>

class csSubMeshWrapper;
struct iIndexBuffer;

CS_CLASS()
class CSE_API csStaticMeshAssetLoader : public CS_SUPER(iAssetLoader)
{
  CS_CLASS_GEN_OBJECT;
public:
  csStaticMeshAssetLoader();
  virtual ~csStaticMeshAssetLoader();

  virtual bool CanLoad(const std::string &typeID, const csResourceLocator &locator, iObject *userData = nullptr) const;
  const csClass *EvalClass(csAssetInputStream &inputStream, const csResourceLocator &locator, iObject *userData = nullptr) const;

  csResourceWrapper *Load(csAssetInputStream &inputStream, const csResourceLocator &locator, iObject *userData = nullptr) const;

private:
  csSubMeshWrapper *ReadSubMesh(csAssetInputStream &inputStream, std::vector<iIndexBuffer*> &globalIndexBuffers, const csResourceLocator &locator, iObject *userData = 0) const;
};
