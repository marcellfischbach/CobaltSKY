
#pragma once

#include <editor/editorexport.hh>

#include <filesystem>
#include <vector>
#include <string>

struct iAssetImporter;

struct EDITOR_API iAssetImporterFactory
{
  virtual ~iAssetImporterFactory() { }

  virtual const std::string GetName() const = 0;

  virtual const std::vector<std::string> GetExtensions() const = 0;

  virtual bool CanImport(const std::filesystem::path &path) const = 0;

  virtual std::vector<iAssetImporter*> Import(const std::filesystem::path &path) const = 0;

};

