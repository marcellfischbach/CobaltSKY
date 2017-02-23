#pragma once


#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkresourcemanager.hh>
#include <shadergrapheditor/shadergrapheditormetaassetxmlloader.refl.hh>

class vkSGNode;
class vkSGShaderGraph;


VK_CLASS()
class ShaderGraphEditorMetaAssetXMLLoader : public VK_SUPER(vkBaseXMLLoader)
{
  VK_CLASS_GEN;
public:
  ShaderGraphEditorMetaAssetXMLLoader();
  virtual ~ShaderGraphEditorMetaAssetXMLLoader();

  virtual bool CanLoad(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData = 0) const;
  virtual const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData = 0) const;
  virtual iObject *Load(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData = 0) const;

};