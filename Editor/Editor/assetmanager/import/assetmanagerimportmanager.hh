#pragma once

#include <vector>

struct AssetManagerImporter;
class AssetManagerImportManager
{
public:
  static AssetManagerImportManager *Get();

  void RegisterImporter(AssetManagerImporter *importer);
  const std::vector<AssetManagerImporter*> &GetImporters() const;

private:
  AssetManagerImportManager();

  std::vector<AssetManagerImporter*> m_importers;
};