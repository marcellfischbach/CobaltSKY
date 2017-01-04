#pragma once


#include <Valkyrie/vkexport.hh>
#include <Valkyrie/Core/vkresourcemanager.hh>
#include <Valkyrie/Loaders/ShaderGraphLoader.refl.hh>

class vkSGNode;
class vkSGShaderGraph;


VK_CLASS()
class VKE_API vkShaderGraphAssetXMLLoader : public VK_SUPER(vkBaseXMLLoader)
{
  VK_CLASS_GEN;
public:
  vkShaderGraphAssetXMLLoader();
  virtual ~vkShaderGraphAssetXMLLoader();

  virtual bool CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;

};