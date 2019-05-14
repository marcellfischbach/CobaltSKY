
#pragma once

#include <editor/editorexport.hh>
#include <csrefl/class.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <QWidget>
#include <string>
#include <filesystem>
#include <editor/iassetimporter.refl.hh>

namespace asset::model
{
  class Folder;
}

CS_CLASS()
struct EDITOR_API iAssetImporter : public cs::iObject
{
  CS_CLASS_GEN;
  iAssetImporter() : cs::iObject() { }
  virtual ~iAssetImporter() { }

  virtual const std::filesystem::path &GetFile() const = 0;

  virtual const std::string GetType() const = 0;

  virtual QWidget *GetWidget() = 0;

  virtual void SetImportName(const std::string &importName) = 0;

  virtual const std::string &GetImportName() const = 0;

  virtual void SetEnabled(bool enable) = 0;

  virtual csResourceLocator Import(const asset::model::Folder *folder) = 0;

};