
#include <editor/assetmanager/import/assetmanagerimportmanager.hh>
#include <editor/assetmanager/import/assetmanagerimporter.hh>

AssetManagerImportManager::AssetManagerImportManager()
{

}


AssetManagerImportManager *AssetManagerImportManager::Get()
{
  static AssetManagerImportManager mgr;
  return &mgr;
}


void AssetManagerImportManager::RegisterImporter(AssetManagerImporter *importer)
{
  m_importers.push_back(importer);
}

const std::vector<AssetManagerImporter*> &AssetManagerImportManager::GetImporters() const
{
  return m_importers;
}


