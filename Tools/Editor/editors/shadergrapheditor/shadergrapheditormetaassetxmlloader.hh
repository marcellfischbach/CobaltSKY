#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <editors/shadergrapheditor/shadergrapheditormetaassetxmlloader.refl.hh>

class csSGNode;
class csSGShaderGraph;


CS_CLASS()
class ShaderGraphEditorMetaAssetXMLLoader : public CS_SUPER(csBaseXMLLoader)
{
  CS_CLASS_GEN;
public:
  ShaderGraphEditorMetaAssetXMLLoader();
  virtual ~ShaderGraphEditorMetaAssetXMLLoader();

  virtual bool CanLoad(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual const csClass *EvalClass(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual iObject *Load(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;

};
