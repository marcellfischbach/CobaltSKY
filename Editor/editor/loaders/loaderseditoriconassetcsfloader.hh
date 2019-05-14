#pragma once

#include <editor/editorexport.hh>
#include <csrefl/class.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <editor/loaders/loaderseditoriconassetcsfloader.refl.hh>

CS_CLASS()
class LoadersEditorIconAssetCSFLoader : public CS_SUPER(csBaseCSFLoader)
{
  CS_CLASS_GEN;
public:
  LoadersEditorIconAssetCSFLoader();
  virtual ~LoadersEditorIconAssetCSFLoader();

  virtual bool CanLoad(const csfEntry *entry, const csResourceLocator &locator, cs::iObject *userData = nullptr) const;
  virtual const cs::Class *EvalClass(const csfEntry *entry, const csResourceLocator &locator, cs::iObject *userData = nullptr) const;
  virtual csResourceWrapper *Load(const csfEntry *entry, const csResourceLocator &locator, cs::iObject *userData = nullptr) const;


};
