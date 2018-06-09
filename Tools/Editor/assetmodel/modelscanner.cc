

#include <assetmodel/modelscanner.hh>
#include <assetmodel/asset.hh>
#include <assetmodel/folder.hh>
#include <assetmodel/model.hh>
#include <assetmodel/root.hh>
#include <assetmodel/vfsentry.hh>
#include <cobalt/core/csvfs.hh>
#include <filesystem>
#include <iostream>

namespace asset::model
{
  ModelScanner::ModelScanner(Model *model)
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

  void ModelScanner::Scan(Folder *folder)
  {
    csResourceLocator l = folder->GetResourceLocator();
    std::string path = csVFS::Get()->GetAbsolutePath(l);
    printf("MS_Scan: %s => %s\n", l.GetText().c_str(), path.c_str());

    for (const std::filesystem::directory_entry &p : std::filesystem::directory_iterator(path))
    {
      const std::filesystem::path &childPath = (const std::filesystem::path&)p;
      if (std::filesystem::is_directory(childPath))
      {
        Folder *childFolder = m_model->CreateFolder(childPath.filename().string());
        folder->Add(childFolder);
        Scan(childFolder);
      }
      else if (std::filesystem::is_regular_file(childPath))
      {
        std::string ext = childPath.extension().string();
        if (ext == std::string(".csf") || ext == std::string(".asset"))
        {
          Asset *asset = m_model->CreateAsset(childPath.filename().string());
          folder->Add(asset);
          Scan(asset);
        }
      }
    }
  }

  void ModelScanner::Scan(Asset *asset)
  {
    csResourceLocator assetLocator = asset->GetResourceLocator();
    printf("MS_Scan: %s\n", assetLocator.GetText().c_str());
  }

}
