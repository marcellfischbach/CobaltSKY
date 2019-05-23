#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/csstaticmeshassetloader.refl.hh>


namespace cs
{
struct iIndexBuffer;
class SubMeshWrapper;

CS_CLASS()
class CSE_API StaticMeshAssetLoader : public CS_SUPER(cs::iAssetLoader)
{
  CS_CLASS_GEN_OBJECT;
public:
  StaticMeshAssetLoader();
  virtual ~StaticMeshAssetLoader();

  virtual bool CanLoad(const std::string &typeID, const cs::ResourceLocator &locator, cs::iObject *userData = nullptr) const;
  const cs::Class *EvalClass(cs::AssetInputStream &inputStream, const cs::ResourceLocator &locator, cs::iObject *userData = nullptr) const;

  cs::ResourceWrapper *Load(cs::AssetInputStream &inputStream, const cs::ResourceLocator &locator, cs::iObject *userData = nullptr) const;

private:
  cs::SubMeshWrapper *ReadSubMesh(cs::AssetInputStream &inputStream, std::vector<cs::iIndexBuffer*> &globalIndexBuffers, const cs::ResourceLocator &locator, cs::iObject *userData = 0) const;
};

}

