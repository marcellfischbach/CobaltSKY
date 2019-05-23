#pragma once

#include <editor/editorexport.hh>
#include <csrefl/class.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <editor/loaders/loaderseditoriconassetcsfloader.refl.hh>

CS_CLASS()
class LoadersEditorIconAssetCSFLoader : public CS_SUPER(cs::BaseCSFLoader)
{
  CS_CLASS_GEN;
public:
  LoadersEditorIconAssetCSFLoader();
  virtual ~LoadersEditorIconAssetCSFLoader();

  virtual bool CanLoad(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData = nullptr) const;
  virtual const cs::Class *EvalClass(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData = nullptr) const;
  virtual cs::ResourceWrapper *Load(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData = nullptr) const;


};
