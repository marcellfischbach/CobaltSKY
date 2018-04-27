#pragma once

#include <string>
#include <map>
#include <vector>
#include <QWidget>

struct iImporter
{

  virtual std::string GetName () const = 0;

  virtual std::string GetAssetName () const = 0;

  virtual QWidget *GetWidget () = 0;
};


struct iImporterFactory
{

    virtual std::map<std::string, std::vector<std::string>> GetImportFormats () const = 0;

    virtual bool CanImport(const std::string &fileName) const = 0;

    virtual iImporter *CreateImporter (const std::string &fileName) const = 0;

};
