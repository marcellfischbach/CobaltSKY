#pragma once

#include <editor/editorexport.hh>
#include <string>
#include <map>
#include <vector>
#include <QWidget>

class AssetManagerWidget;

struct iImporter
{

  virtual std::string GetName () const = 0;

  virtual std::string GetAssetName () const = 0;

  virtual std::string GetFileName () const = 0;

  virtual QWidget *GetWidget () = 0;

  virtual bool Import(AssetManagerWidget *assetManagerWidget) = 0;
};


struct iImporterFactory
{

    virtual std::map<std::string, std::vector<std::string>> GetImportFormats () const = 0;

    virtual bool CanImport(const std::string &fileName) const = 0;

    virtual iImporter *CreateImporter (const std::string &fileName) const = 0;

};
