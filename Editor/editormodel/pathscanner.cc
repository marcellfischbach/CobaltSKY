
#include <editormodel/pathscanner.hh>
#include <editormodel/model.hh>
#include <editormodel/nodescanner.hh>
#include <cobalt/core/csvfs.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <filesystem>
#include <iostream>

namespace std
{
namespace fs = filesystem;
}

namespace cs::editor::model
{

PathScanner::PathScanner(Model *model)
  : m_model(model)
{
}


void PathScanner::Scan(VFSEntryNode *entry, Transaction &tx)
{
  if (!entry)
  {
    return;
  }

  std::string path = cs::VFS::Get()->GetAbsolutePath(entry->GetResourceLocator(), cs::VFS::CheckExistence);
  if (path.empty())
  {
    return;
  }

  // printf("Scan: '%s' -> '%s'\n", entry->GetEntry()->GetName().c_str(), entry->GetResourceLocator().Encode().c_str());
  ScanFolder(entry, std::fs::path(path), tx);
}


void PathScanner::ScanFolder(FolderNode *parentNode, const std::fs::path &path, Transaction &tx)
{

  for (const std::fs::path &path : std::fs::directory_iterator(path))
  {
    Scan(parentNode, path, tx);
  }
}

void PathScanner::Scan(FolderNode *parentNode, const std::fs::path &path, Transaction &tx)
{
  Model *model = parentNode->GetModel();
  //printf("   %s\n", path.generic_string().c_str());
  if (std::fs::is_regular_file(path))
  {
    cs::ResourceLocator loc = parentNode->GetResourceLocator().WithFileSuffix(path.filename().generic_string());
    std::string locEnc = loc.Encode();
    const cs::Class * cls = cs::ResourceManager::Get()->EvalClass(loc);
    if (cls)
    {
      AssetNode *assetNode = model->CreateAssetNode();
      assetNode->SetName(path.filename().generic_string());
      assetNode->SetAssetClass(cls);
      model->Add(assetNode, parentNode, tx);
      if (path.extension() == ".csf" || path.extension() == ".asset")
      {
        NodeScanner nodeScanner;
        nodeScanner.Scan(assetNode);
      }
    }
  }
  else if (std::fs::is_directory(path))
  {
    //printf("  Directory: %s\n", path.filename().generic_string().c_str());

    FolderNode *folderNode = model->CreateFolderNode();
    folderNode->SetName(path.filename().generic_string());
    model->Add(folderNode, parentNode, tx);
    ScanFolder(folderNode, path, tx);
  }

}


}
