

#include <editor/assetmodel/sync/modelscanner.hh>
#include <editor/assetmodel/sync/assetscanner.hh>
#include <editor/assetmodel/asset.hh>
#include <editor/assetmodel/folder.hh>
#include <editor/assetmodel/model.hh>
#include <editor/assetmodel/modeltransaction.hh>
#include <editor/assetmodel/root.hh>
#include <editor/assetmodel/vfsentry.hh>
#include <cobalt/core/csvfs.hh>
#include <filesystem>
#include <iostream>

namespace std
{
  namespace fs = filesystem;
}

namespace asset::model
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
    cs::VFS *vfs = cs::VFS::Get();
    Root *root = m_model->GetRoot();
    for (csSize i = 0, in = vfs->GetNumberOfEntries(); i < in; ++i)
    {
      const cs::VFS::Entry &entry = vfs->GetEntry(i);
      VFSEntry *vfsEntry = m_model->CreateVFSEntry(entry);
			ModelTransaction tr;
			try
			{
				root->Add(vfsEntry, tr);
			}
			catch (const std::exception &e)
			{

			}
			tr.Commit();
      Scan(vfsEntry);

    }
  }

  void ModelScanner::Scan(asset::model::Folder *folder)
  {
    cs::ResourceLocator l = folder->GetResourceLocator();
    std::string path = cs::VFS::Get()->GetAbsolutePath(l);
    printf("MS_Scan: %s => %s\n", l.Encode().c_str(), path.c_str());

    for (const std::fs::directory_entry &p : std::fs::directory_iterator(path))
    {
      const std::fs::path &childPath = (const std::fs::path&)p;
      if (std::fs::is_directory(childPath))
      {
        asset::model::Folder *childFolder = m_model->CreateFolder(childPath.filename().string());
				ModelTransaction tr;
				try
				{
					folder->Add(childFolder, tr);
				}
				catch (const std::exception &e)
				{

				}
				tr.Commit();
        Scan(childFolder);
      }
      else if (std::fs::is_regular_file(childPath))
      {
        std::string ext = childPath.extension().string();
        if (ext == std::string(".csf") || ext == std::string(".asset"))
        {
          asset::model::Asset *asset = m_model->CreateAsset(childPath.filename().string());
					ModelTransaction tr;
					try
					{
						folder->Add(asset, tr);
					}
					catch (const std::exception &e)
					{

					}
					tr.Commit();
          Scan(asset);
        }
      }
    }
  }

  void ModelScanner::Scan(asset::model::Asset *asset)
  {
    cs::ResourceLocator assetLocator = asset->GetResourceLocator();
    std::fs::path path(cs::VFS::Get()->GetAbsolutePath(assetLocator));
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

    printf("MS_Scan: %s\n", assetLocator.Encode().c_str());
  }

}
