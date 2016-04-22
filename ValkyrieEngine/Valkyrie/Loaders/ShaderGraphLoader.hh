#pragma once


#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/ResourceManager.hh>
#include <Valkyrie/Loaders/ShaderGraphLoader.refl.hh>

class vkSGNode;
class vkSGShaderGraph;

VK_CLASS()
class VKE_API vkShaderGraphAssetLoader : public IAssetLoader
{
  VK_CLASS_GEN_OBJECT;
public:
  vkShaderGraphAssetLoader();
  virtual ~vkShaderGraphAssetLoader();

  virtual bool CanLoad(vkAssetInputStream &inputStream, const vkResourceLocator &locator, IObject *userData = 0) const;

  virtual IObject *Load(vkAssetInputStream &inputStream, const vkResourceLocator &locator, IObject *userData = 0) const;

private:
  void Cleanup(std::map<vkUInt32, vkSGNode *> &nodes, vkSGShaderGraph *graph) const;
};