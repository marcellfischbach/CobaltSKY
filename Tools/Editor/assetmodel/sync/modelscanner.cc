

#include <assetmodel/sync/modelscanner.hh>
#include <assetmodel/sync/assetscanner.hh>
#include <assetmodel/asset.hh>
#include <assetmodel/folder.hh>
#include <assetmodel/model.hh>
#include <assetmodel/root.hh>
#include <assetmodel/vfsentry.hh>
#include <cobalt/core/csvfs.hh>
#include <filesystem>
#include <iostream>

namespace std
{
  namespace fs = filesystem;
}

namespace asset::model::sync
{
  ModelScanner::ModelScanner(asset::model::Model *model)
    : m_model(model)
  {

  }

  ModelScanner::~ModelScanner()
  {

  }


  void ModelScanner::Scan()
  {
    csVFS *vfs = csVFS::Get();
    Root *root = m_model->GetRoot();
    for (csSize i = 0, in = vfs->GetNumberOfEntries(); i < in; ++i)
    {
      const csVFS::Entry &entry = vfs->GetEntry(i);
      VFSEntry *vfsEntry = m_model->CreateVFSEntry(entry);
      root->Add(vfsEntry);
      Scan(vfsEntry);

    }
  }

  void ModelScanner::Scan(asset::model::Folder *folder)
  {
    csResourceLocator l = folder->GetResourceLocator();
    std::string path = csVFS::Get()->GetAbsolutePath(l);
    printf("MS_Scan: %s => %s\n", l.GetText().c_str(), path.c_str());

    for (const std::fs::directory_entry &p : std::fs::directory_iterator(path))
    {
      const std::fs::path &childPath = (const std::fs::path&)p;
      if (std::fs::is_directory(childPath))
      {
        asset::model::Folder *childFolder = m_model->CreateFolder(childPath.filename().string());
        folder->Add(childFolder);
        Scan(childFolder);
      }
      else if (std::fs::is_regular_file(childPath))
      {
        std::string ext = childPath.extension().string();
        if (ext == std::string(".csf") || ext == std::string(".asset"))
        {
          asset::model::Asset *asset = m_model->CreateAsset(childPath.filename().string());
          folder->Add(asset);
          Scan(asset);
        }
      }
    }
  }

  void ModelScanner::Scan(asset::model::Asset *asset)
  {
    csResourceLocator assetLocator = asset->GetResourceLocator();
    std::fs::path path(csVFS::Get()->GetAbsolutePath(assetLocator));
    std::string ext = path.extension().string();
    if (ext == std::string(".csf") || ext == std::string(".asset"))
    {
      AssetScanner(asset).Scan();
    }
    else
    {
      if (ext.length() != 0 && ext[0] == '.')
      {
        ext = ext.substr(1);
      }
      asset->SetAssetType(ext);
    }

    printf("MS_Scan: %s\n", assetLocator.GetText().c_str());
  }

}
