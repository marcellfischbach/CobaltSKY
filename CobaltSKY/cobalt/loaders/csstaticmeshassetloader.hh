#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/csstaticmeshassetloader.refl.hh>

class csSubMesh;
struct iIndexBuffer;

CS_CLASS()
class CSE_API csStaticMeshAssetLoader : public CS_SUPER(iAssetLoader)
{
  CS_CLASS_GEN_OBJECT;
public:
  csStaticMeshAssetLoader();
  virtual ~csStaticMeshAssetLoader();

  virtual bool CanLoad(const csString &typeID, const csResourceLocator &locator, iObject *userData = 0) const;
  const csClass *EvalClass(csAssetInputStream &inputStream, const csResourceLocator &locator, iObject *userData = 0) const;

  iObject *Load(csAssetInputStream &inputStream, const csResourceLocator &locator, iObject *userData = 0) const;

private:
  csSubMesh *ReadSubMesh(csAssetInputStream &inputStream, std::vector<iIndexBuffer*> &globalIndexBuffers, const csResourceLocator &locator, iObject *userData = 0) const;
};