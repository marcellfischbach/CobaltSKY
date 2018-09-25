
#pragma once

#include <editor/editorexport.hh>
#include <cobalt/core/csclass.hh>
#include <QWidget>
#include <string>
#include <filesystem>
#include <editor/iassetimporter.refl.hh>


CS_INTERFACE()
struct EDITOR_API iAssetImporter : public iObject
{
  CS_CLASS_GEN;
  iAssetImporter() : iObject() { }
  virtual ~iAssetImporter () { }

  virtual const std::filesystem::path &GetFile() const = 0;

  virtual const std::string GetType() const = 0;

  virtual QWidget *GetWidget() = 0;

  virtual void SetEnabled(bool enable) = 0;

  virtual void Import() = 0;

};