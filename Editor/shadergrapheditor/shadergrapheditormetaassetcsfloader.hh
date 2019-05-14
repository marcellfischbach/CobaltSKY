#pragma once


#include <shadergrapheditor/shadergrapheditorexport.hh>
#include <cobalt/csexport.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <shadergrapheditor/shadergrapheditormetaassetcsfloader.refl.hh>

class csSGNode;
class csSGShaderGraph;


CS_CLASS()
class ShaderGraphEditorMetaAssetCSFLoader : public CS_SUPER(csBaseCSFLoader)
{
  CS_CLASS_GEN;
public:
  ShaderGraphEditorMetaAssetCSFLoader();
  virtual ~ShaderGraphEditorMetaAssetCSFLoader();

  virtual bool CanLoad(const csfEntry *entry, const csResourceLocator &locator, cs::iObject *userData = nullptr) const;
  virtual const cs::Class *EvalClass(const csfEntry *entry, const csResourceLocator &locator, cs::iObject *userData = nullptr) const;
  virtual csResourceWrapper *Load(const csfEntry *entry, const csResourceLocator &locator, cs::iObject *userData = nullptr) const;

};
