#pragma once


#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/ResourceManager.hh>
#include <Valkyrie/Loaders/ShaderGraphLoader.refl.hh>

class vkSGNode;
class vkSGShaderGraph;


VK_CLASS()
class VKE_API vkShaderGraphAssetXMLLoader : public vkBaseXMLLoader
{
  VK_CLASS_GEN;
public:
  vkShaderGraphAssetXMLLoader();
  virtual ~vkShaderGraphAssetXMLLoader();

  virtual bool CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;

};